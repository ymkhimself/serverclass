package main

import (
	"fmt"
	"io"
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"strings"
	"time"
)

const LIST_PATH = "/home/ymk/server/lab2/list"
const DST_PATH = "/home/ymk/server/backup"

var SRC_PATH string

var list *os.File

func main() {
	if len(os.Args) != 2 {
		log.Fatal("请输入要备份的文件夹\n")
	}
	SRC_PATH = os.Args[1]
	timeTickerChan := time.Tick(time.Second * 4) //四秒一备份
	for {
		<-timeTickerChan
		backupData(SRC_PATH)
	}
}

func backupData(path string) {
	var err error
	list, err = os.OpenFile(LIST_PATH, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0664)
	defer list.Close()
	if err != nil {
		log.Fatal(err.Error())
	}
	log.Println("开始备份")
	fmt.Fprintf(list, "%s 开始备份\n", time.Now().Format("2006-01-02 15:04:05"))
	err = filepath.Walk(path, walkHandler)
	if err != nil {
		log.Fatal(err.Error())
	}
}

func walkHandler(path string, info fs.FileInfo, err error) error {
	if err != nil {
		return err
	}
	if info.IsDir() {
		dir := changePath(path)
		fmt.Fprintf(list, "%s 创建目录:%s\n", dir, time.Now().Format("2006-01-02 15:04:05"))
		_, err = os.Stat(dir)
		if err != nil {
			if err := os.Mkdir(dir, 0775); err != nil {
				return err
			}
		}
	} else {
		newPath := changePath(path)
		err = copyFile(path, newPath)
		if err != nil {
			log.Fatal("拷贝出错\n")
		}
		fmt.Fprintf(list, "%s 拷贝文件:%s\n", newPath, time.Now().Format("2006-01-02 15:04:05"))
	}
	return nil
}

// 输入原路径，返回调整过的路径。
func changePath(src string) string {
	return strings.Replace(src, SRC_PATH, DST_PATH, 1)
}

func copyFile(src_path, dst_path string) error {
	src, err := os.Open(src_path)
	if err != nil {
		return err
	}
	dst, err := os.Create(dst_path)
	if err != nil {
		return err
	}
	_, err = io.Copy(dst, src)
	if err != nil {
		return err
	}
	return nil
}
