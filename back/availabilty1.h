int set_count(int type){
    int account_file_fd = open("./account",O_RDWR);
    if (account_file_fd == -1) {
        exit(1);
    }
    int offset = lseek(account_file_fd ,0*sizeof(struct record) ,SEEK_SET );
    if(offset != -1){
            struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, getpid()};
            // lock.l_type = F_WRLCK;
            // lock.l_whence = SEEK_SET;
            // lock.l_start = 0;
            // lock.l_len = 0;
            // lock.l_pid = getpid();
            int locking_stat = fcntl(account_file_fd, F_SETLKW, &lock);
            if(locking_stat == -1){
                perror("Error obtaining write lock on Count Record!");
                close(account_file_fd);
            }
            struct record rec;
            read(account_file_fd ,&rec ,sizeof(struct record) );
            lseek(account_file_fd,0*sizeof(struct record),SEEK_SET);
            if(type == 1)
                rec.student++;
            else if(type == 2)
                rec.teacher++;
            else if(type == 3) 
                rec.courses++;
            else rec.enrolls++;
            
            write(account_file_fd ,&rec ,sizeof(rec) );
            // Unlocking
            lock.l_type = F_UNLCK;
            fcntl(account_file_fd,F_SETLK,&lock);
            close(account_file_fd);
            if(type == 1)
                return rec.student;
            else if(type == 2)
                return rec.teacher;
            else if(type == 3) 
                return rec.courses;
            else 
                return rec.enrolls;
    }
}

int get_count(int type){
    int account_file_fd = open("./account",O_RDONLY);
    if (account_file_fd == -1) {
        exit(1);
    }
    int offset = lseek(account_file_fd ,0*sizeof(struct record) ,SEEK_SET);
    if(offset != -1){
            struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
            // lock.l_type = F_RDLCK;
            // lock.l_whence = SEEK_SET;
            // lock.l_start = 0;
            // lock.l_len = 0;
            // lock.l_pid = getpid();
            int locking_stat = fcntl(account_file_fd, F_SETLKW, &lock);
            if(locking_stat == -1){
                perror("Error obtaining Read lock on Count Record!");
                close(account_file_fd);
            }
            struct record rec;
            read(account_file_fd ,&rec ,sizeof(rec));
            // Unlocking
            lock.l_type = F_UNLCK;
            fcntl(account_file_fd ,F_SETLK ,&lock);
            close(account_file_fd);
            if(type == 1)
                return rec.student;
            else if(type == 2)
                return rec.teacher;
            else if(type == 3) 
                return rec.courses;
            else 
                return rec.enrolls;           
    }
}

struct course get_course(int desc, int id){
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024];
    struct course c;
    int course_file_fd = open("./courses",O_RDONLY);
    if (course_file_fd == -1) {
        exit(1);
    }
    if(get_count(3) < id){
        write(desc,"Wrong Course id.^\n",sizeof("Wrong Course id.^\n"));
        return c;
    }
    struct flock lock = {F_RDLCK, SEEK_SET, (id-1)*sizeof(struct course), sizeof(struct course), getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = (id-1)*sizeof(struct course);
    // lock.l_len = sizeof(struct course);
    // lock.l_pid = getpid();
    int locking_stat = fcntl(course_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("Error obtaining Read lock on Courses Record!");
        close(course_file_fd);
    }
    lseek(course_file_fd ,(id-1) * sizeof(struct course) ,SEEK_SET);
    // int bytesRead;
    read(course_file_fd ,&c ,sizeof(struct course));
    lock.l_type = F_UNLCK;
    fcntl(course_file_fd,F_SETLK,&lock);
    close(course_file_fd);
    return c;
}

// void set_availability(int desc, int id,int cnt){
//     int fd = open("./courses",O_RDWR);
//     if (fd == -1) {
//         exit(1);
//     }
//     if(get_count(3)<id){
//         write(desc,"Wrong Course id\n",sizeof("Wrong Course id\n"));
//     }
//     struct flock lock;
//     lock.l_type = F_WRLCK;
//     lock.l_whence = SEEK_SET;
//     lock.l_start = (id-1)*sizeof(struct course);
//     lock.l_len = sizeof(struct course);
//     lock.l_pid = getpid();
//     int f = fcntl(fd, F_SETLKW, &lock);
//     if(f==-1){
//         perror("Error obtaining Read lock on Courses Record!");
//         close(fd);
//     }
//     lseek(fd , (id-1) * sizeof(struct course), SEEK_SET);
//     struct course cr;
//     read(fd ,&cr ,sizeof(struct course));
//     cr.available += cnt;
//     lseek(fd ,(id-1) * sizeof(struct course), SEEK_SET);
//     write(fd ,&cr , sizeof(struct course));
//     lock.l_type = F_UNLCK;
//     fcntl(fd,F_SETLK,&lock);
//     close(fd);
//     return;
// }
