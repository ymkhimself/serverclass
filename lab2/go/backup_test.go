package main

import (
	"fmt"
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"strings"
	"testing"
	"time"
)

func TestChangePath(t *testing.T) {
	DST_PATH := "/home/ymk/server/lab2/backup"
	SRC_PATH := "home/ymk/server/lab2/data"
	path := "home/ymk/server/lab2/data/asdklj/asdjkl/a.c"
	p := strings.Replace(path, SRC_PATH, DST_PATH, 1)
	fmt.Println(p)
}

func TestCopyFile(t *testing.T) {
	src_path := "/home/ymk/server/data/a.c"
	dst_path := "/home/ymk/server/backup/a.c"
	err := copyFile(src_path, dst_path)
	if err != nil {
		log.Println(err.Error())
	}
}

func TestWriteToList(t *testing.T) {
	LIST_PATH := "/home/ymk/server/lab2/list"
	list, err := os.OpenFile(LIST_PATH, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0664)
	if err != nil {
		log.Fatal(err.Error())
	}
	list.WriteString("asd")

	fmt.Fprintf(list, "%s 开始备份\n", time.Now().Format("2006-01-02 15:04:05"))
}

func TestWalk(t *testing.T) {
	SRC_PATH := "/home/server/data"
	filepath.Walk(SRC_PATH, func(path string, info fs.FileInfo, err error) error {
		log.Println(path)
		return nil
	})
}
