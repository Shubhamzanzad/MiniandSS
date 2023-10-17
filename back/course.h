struct course get_courses(int desc, int id){
    struct course c;
    int course_file_fd = open("./courses",O_RDONLY);
    if (course_file_fd == -1) {
        _exit(0);
    }
    if(get_count(3)<id){
        write(desc,"Wrong Course id.^",sizeof("Wrong Course id.^"));
        return c;
    }
    struct flock lock = {F_RDLCK, SEEK_SET, (id-1)*sizeof(struct course), sizeof(struct course), getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = ;
    // lock.l_len = sizeof(struct course);
    // lock.l_pid = getpid();
    int locking_stat = fcntl(course_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("Error obtaining Read lock on Courses Record!");
        close(course_file_fd);
    }
    lseek(course_file_fd ,(id-1)*sizeof(struct course) ,SEEK_SET);
    // int bytesRead;
    read(course_file_fd, &c ,sizeof(struct course));
    lock.l_type = F_UNLCK;
    fcntl(course_file_fd,F_SETLK,&lock);
    close(course_file_fd);
    return c;
}

void set_availability(int desc, int id,int cnt){
    int course_file_fd = open("./courses",O_RDWR);
    if (course_file_fd == -1) {
        exit(1);
    }
    if(get_count(3)<id){
        write(desc,"Wrong Course id.^",sizeof("Wrong Course id.^"));
    }
    struct flock lock = {F_WRLCK, SEEK_SET, (id-1)*sizeof(struct course), sizeof(struct course), getpid()};
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = ;
    // lock.l_len = sizeof(struct course);
    // lock.l_pid = getpid();
    int locking_stat = fcntl(course_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("Error obtaining Read lock on Courses Record!");
        close(course_file_fd);
    }
    lseek(locking_stat ,(id-1)*sizeof(struct course) ,SEEK_SET );
    struct course c;
    int bytesRead;
    read(course_file_fd ,&c ,sizeof(struct course) );
    c.available += cnt;
    lseek(course_file_fd ,(id-1)*sizeof(struct course) ,SEEK_SET );
    write(course_file_fd ,&c ,sizeof(struct course) );
    lock.l_type = F_UNLCK;
    fcntl(course_file_fd ,F_SETLK ,&lock );
    close(course_file_fd);
    return;
}