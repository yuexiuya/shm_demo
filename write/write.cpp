#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define SHM_NAME "shm_ram"
#define SHM_SIZE 4096*4

int main(int argc, char *argv[])
{
    printf("[LOG] -- write start -- \n");

    /*(1) 创建或者打开一个共享内存*/
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
    void* add_w= mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, SEEK_SET);
    if(add_w == NULL) {
        printf("[ERROR] %s failed !!! \n","mmap");
        return -1;
    } else {
        printf("[LOG] %s success !!! \n","mmap");
    }

    /*(4) memcpy 内存共享 写入内容*/
    const char* _tmp = "joshua";
    printf("[LOG] write context = %s\n",_tmp);
    memcpy(add_w, _tmp, strlen(_tmp)+1);

    while(1);

     /*下面会销毁共享内存区域*/
     /*(5) 取消映射*/
     ret = munmap(add_w, SHM_SIZE);
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

     printf("[LOG] -- write end -- \n");
     return 0;
}
