#include "backup.h"

// 实现方案,遍历目录，一边遍历一边拷贝,然后将拷贝好的文件记录到list里面

void copyfile(const char *src, const char *dst) {
    char *buffer = (char *)malloc(BUF_SIZE);
    FILE *source = fopen(src, "r");
    FILE *target = fopen(dst, "w+");
    size_t s;
    while ((s = fread(buffer, 1, BUF_SIZE, source)) != 0) {
        fwrite(buffer, 1, s, target);
    }

    fclose(source);
    fclose(target);
}

void changePath(char *org, char *dst) {
    char buffer[255] = {0};
    int len = strlen(SRC_DIR);
    memcpy(buffer, org + len, strlen(org) - len);
    stpcpy(dst, DST_DIR);
    strcat(dst, buffer);
}

void backupData(const char *dir, FILE *LIST) {
    DIR *dp;
    struct dirent *drip;

    char PATH_ORG[255] = {0};
    char PATH_DST[255] = {0};

    if (NULL == (dp = opendir(dir)))
        return;

    while (NULL != (drip = readdir(dp))) {
        if (drip->d_type == DT_DIR) {
            // 是一个目录，需要修改路径,重新创建
            strcpy(PATH_ORG, dir);
            checkSp(drip->d_name);
            strcat(PATH_ORG, "/");
            strcat(PATH_ORG, drip->d_name);
            puts(PATH_ORG);
            changePath(PATH_ORG, PATH_DST);
            puts(PATH_DST);
            mkdir(PATH_DST, 0775);
            fprintf(LIST, "创建目录%s\n", PATH_DST);
            backupData(PATH_ORG, LIST);
        } else {
            // 是一个文件，需要修改路径，进行，拷贝文件。
            strcpy(PATH_ORG, dir);
            strcat(PATH_ORG, "/");
            strcat(PATH_ORG, drip->d_name);
            changePath(PATH_ORG, PATH_DST);
            copyfile(PATH_ORG, PATH_DST);
            fprintf(LIST, "拷贝文件%s\n", PATH_DST);
        }
    }
    free(dp);
}

void handler() {
    // 追加模式打开list文件，不存在时将自动创建
    FILE *LIST = fopen(LIST_PATH, "a");
    time_t t = time(0);
    char tmpBuf[255];
    strftime(tmpBuf, 255, "%Y-%m-%d %H:%M:%S", localtime(&t));
    log("%s Backup begin\n",tmpBuf);
    fprintf(LIST, "%s 开始\n",tmpBuf);
    backupData(SRC_DIR, LIST);
    fclose(LIST);
    alarm(CYCLE_TIME);
}
