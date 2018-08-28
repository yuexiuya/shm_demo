#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#ifdef MMAP

#define SHM_NAME "shm_ram"
#define SHM_SIZE 4096*4

int main(int argc, char *argv[])
{
    printf("[LOG] -- read start -- \n");
    /* (1) 创建或打开一个共享库 */
    int fd = shm_open(SHM_NAME, O_RDWR|O_CREAT, 0777);
    if (fd==-1) {
        printf("[ERROR] %s failed !!! \n","shm_open");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shm_open");
    }
    /* (2) 调整共享库的大小*/
    int ret = ftruncate(fd, SHM_SIZE);
    if( ret != 0) {
        printf("[ERROR] %s failed !!! \n","ftruncate");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","ftruncate");
    }

    /* (3) 映射目标文件的存储区 */
    void* add_r = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, SEEK_SET);
    if(add_r == NULL) {
        printf("[ERROR] %s failed !!! \n","mmap");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","mmap");
    }

    /*(4) memcpy 内存共享 写入内容*/
    char buf[4096] = {0};
    memcpy(buf, add_r, sizeof(buf));
    printf("[LOG] read buf = %s \n", buf);

    /*(5) 取消映射*/
    ret = munmap(add_r, SHM_SIZE);
    if(ret == -1) {
        printf("[ERROR] %s failed !!! \n","munmap");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","munmap");
    }

    /*(6) 删除内存共享*/
    ret = shm_unlink(SHM_NAME);
    if(ret == -1) {
        printf("[ERROR] %s failed !!! \n","shm_unlink");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shm_unlink");
    }

    printf("[LOG] -- read end -- \n");
    return 0;
}

#endif


#ifdef SHM_GET
#include <string>
#include <sys/shm.h>
#define FILE_SIZE 1024
typedef struct _MediaInfo MediaInfo;

struct _MediaInfo {
    char title[FILE_SIZE];
    char artist[FILE_SIZE];
    char album[FILE_SIZE];
    int size;
};

int main(int argc, char *argv[]) {
    MediaInfo info;
    memset(&info, 0 , sizeof(info));
    /* (1) 创建共享内存 */
    int shmid = shmget((key_t)1234, sizeof(info), 0666|IPC_CREAT);
    if( shmid == -1) {
        printf("[ERROR] %s failed !!! \n","shmget");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shmget");
    }

    /* (2) 将共享内存连接到当前进程的地址空间*/
    void *pshm = shmat(shmid, NULL, 0);
    if(pshm == (void *)-1) {
        printf("[ERROR] %s failed !!! \n","shmat");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shmat");
    }

    printf("[LOG] Memory attached at %lx !!! \n",(long)pshm);

    /*(3) 从共享内存中获取数据*/
    memcpy(&info, pshm, sizeof(info));

    printf("[LOG] Memory attached at %lx !!! \n",(long)pshm);
    printf("[LOG] artist = %s !!! \n",info.artist);
    printf("[LOG] title = %s !!! \n",info.title);
    printf("[LOG] album = %s !!! \n",info.album);
    printf("[LOG] size = %d !!! \n", info.size);

    /*(4) 把共享内存从当前进程中分离*/
    int ret = shmdt(pshm);
    if( ret == -1) {
        printf("[ERROR] %s failed !!! \n","shmdt");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shmdt");
    }

    /*(5) 删除共享内存*/
    ret = shmctl(shmid, IPC_RMID, 0);
    if( ret == -1) {
        printf("[ERROR] %s failed !!! \n","shmctl");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","shmctl");
    }

    return 0;
}

#endif
