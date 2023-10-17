int* view_offering_courses(int desc, int id, int *courseCount, int flag){
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int courses_file_fd = open("./courses",O_RDONLY);
    if (courses_file_fd == -1) {
        exit(1);
    }
    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    int locking_stat = fcntl(courses_file_fd, F_SETLKW, &lock);
    if(courses_file_fd == -1){
        perror("Error obtaining Read lock on Courses Record!");
        close(courses_file_fd);
    }
    lseek(courses_file_fd ,0 ,SEEK_SET);
    struct course c;
    int arraySize = 1;
    int count = 0;
    int *cid = (int *)malloc(arraySize * sizeof(int));
    while((read_size = read(courses_file_fd ,&c ,sizeof(struct course))) == sizeof(struct course) || read_size == -1){
        if(c.prof_id == id){
            if(flag){
            char buff[100+sizeof(struct course)], skip[2];
            bzero(buff, sizeof(buff));
            sprintf(buff ,"Course Name: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d.^",
            c.course_name,c.credit,c.seats,c.available,c.course_id);
            write_size = write(desc ,buff ,sizeof(buff));
            if(write_size == -1) {
                perror("ERROR");
            }
            bzero(buff, sizeof(buff));
            read(desc,skip,2);
            }
            arraySize += 1;
            cid = (int *)realloc(cid, arraySize * sizeof(int));
            if (cid == NULL) {
                perror("Memory reallocation failed");
            }
            cid[count++] = c.course_id;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(courses_file_fd ,F_SETLK ,&lock);
    close(courses_file_fd);
    *courseCount=count;
    return cid;
}

struct enroll* view_enrollments(int desc, int id , int *enrollCount,int flag){
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int enroll_file_fd = open("./enroll",O_RDONLY);
    if (enroll_file_fd == -1) {
        _exit(0);
    }
    int courseCount=0,arraySize=1,count=0;
    struct enroll *e = (struct enroll *)malloc(arraySize * sizeof(struct enroll));
    
    int *cids=view_offering_courses(desc,id,&courseCount,0);
    if(courseCount == 0){
        write(desc,"No Courses Created.^",sizeof("No Courses Created.^"));
        return e;
    }
    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
    // lock.l_type = F_RDLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    int locking_stat = fcntl(enroll_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("Error obtaining Read lock on Courses Record!");
        close(enroll_file_fd);
    }    
    for(int i=0 ;i<courseCount ;i++){
        struct course c = get_courses(desc ,cids[i]);
        char send[100+sizeof(struct course)],skip[2];
        if(flag){
                bzero(send, sizeof(send));
                sprintf(send,"Course Name: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d.^",
                c.course_name,c.credit,c.seats,c.available,c.course_id);
                write_size = write(desc ,send ,sizeof(send));
                if(write_size == -1) {
                    perror("ERROR WRITING MESSAGE TO CLIENT");
                    _exit(0);
                }
                read_size = read(desc ,read_buff, sizeof(read_buff));//dummy read
            }
        struct enroll er;
        lseek(enroll_file_fd ,0 ,SEEK_SET);
        int ec = 0;
        while((read_size = read(enroll_file_fd ,&er ,sizeof(struct enroll))) == sizeof(struct enroll) || read_size == -1){
            if(er.is_enrolled && er.course_id == c.course_id && er.prof_id == id){
                char sk[2];
                if(flag){
                    write(desc,".........................Enrollment Details.........................^",
                    sizeof(".........................Enrollment Details.........................^"));
                    view_participant(desc ,1 ,er.stud_id);
                    read(desc ,read_buff ,sizeof(read_buff));//dummy read
                }
                arraySize += 1;
                e = (struct enroll *)realloc(e, arraySize * sizeof(struct enroll));
                if (e == NULL) {
                    perror("Memory reallocation failed");
                }
                e[count++] = er;
                ec++;
            }
        }
        if(ec == 0){
            char sk[2];
            write(desc,".........................Enrollment Details.........................^",
            sizeof(".........................Enrollment Details.........................^"));
            write(desc,"No Enrollments for this course.^",sizeof("No Enrollments for this course.^"));
            read(desc,sk,2);
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(enroll_file_fd,F_SETLK,&lock);
    close(enroll_file_fd);
    *enrollCount = count;
    return e; 
}

void add_new_course(int desc, int id) {
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024], temp_buff[1024];
    int courses_file_fd = open("./courses", O_RDWR | O_APPEND);
    if(courses_file_fd == -1) {
        perror("ERROR OPENING FILE");
        _exit(0);
    }
    struct flock lock = {F_WRLCK, SEEK_END, 0, sizeof(struct course), getpid()};

    int locking_stat = fcntl(courses_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1) {
        perror("ERROR LOCKING THE COURSE FILE");
        _exit(0);
    }
    
    struct course cr;
    char seats[4], credits[2];
    cr.course_id = set_count(3);
    write_size = write(desc, ASK_NAME, sizeof(ASK_NAME));
    if(write_size == -1) {
        perror("ERROR IN ASKING NAME TO CLIENT");
        _exit(0);
    }
    bzero(read_buff, sizeof(read_buff));
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR IN READING FROM CLIENT");
        _exit(0);
    }
    strcpy(cr.course_name, read_buff);

    write_size = write(desc, "Enter credits for course: ", sizeof("Enter credits for course: "));
    if(write_size == -1) {
        perror("ERROR IN ASKING NAME TO CLIENT");
        _exit(0);
    }
    bzero(read_buff, sizeof(read_buff));
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR IN READING FROM CLIENT");
        _exit(0);
    }
    cr.credit = atoi(read_buff);

    write_size = write(desc, "Enter total seats for course: ", sizeof("Enter total for course: "));
    if(write_size == -1) {
        perror("ERROR IN ASKING NAME TO CLIENT");
        _exit(0);
    }
    bzero(read_buff, sizeof(read_buff));
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("ERROR IN READING FROM CLIENT");
        _exit(0);
    }
    cr.credit = atoi(read_buff);
    cr.prof_id = id;
    write_size = write(courses_file_fd, &cr, sizeof(struct course));
    if(write_size == -1) {
        perror("ERROR COULD NOT WRITE NEW COURSE INFORMATION IN COURSE FILE");
        _exit(0);
    }
    write_size = write(desc,"New course info added to course file.^",sizeof("New course info added to course file.^"));
    if(write_size == -1) {
        perror("Could not pass msg");
        _exit(0);
    }
    bzero(temp_buff, sizeof(temp_buff));
    sprintf(temp_buff, "\nCourse ID: %d.^", cr.course_id);
    write(desc, temp_buff, sizeof(temp_buff));
    lock.l_type = F_UNLCK;
    fcntl(courses_file_fd, F_SETLK, &lock);
    close(courses_file_fd);
    read(desc, read_buff, sizeof(read_buff));
}

int handle_seats(int desc, int id,int cid, int prevSeats, int currSeats, int available){
    if(prevSeats <= currSeats)
        return currSeats-prevSeats; 
    else if(prevSeats > currSeats){
        int seatsDecr = prevSeats - currSeats;
        if(available >= seatsDecr)
            return -seatsDecr;
        else{
        int unEnrollNum = seatsDecr - available;
        int enrollCnt;
        struct enroll *e = view_enrollments(desc,id,&enrollCnt,0);
        int struct_size = sizeof(struct enroll);

        int enroll_file_fd = open("./enroll",O_RDWR);
        if (enroll_file_fd == -1) {
            _exit(0);
        }
        // Un-enrolling students from deleted course
        struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, getpid()};
        // lock.l_type = F_WRLCK;
        // lock.l_whence = SEEK_SET;
        // lock.l_start = 0;
        // lock.l_len = 0;
        // lock.l_pid = getpid();
        int locking_status = fcntl(enroll_file_fd, F_SETLKW, &lock);
        if(locking_status == -1){
            perror("Error obtaining Read lock on Courses Record!");
            close(enroll_file_fd);
        }

        int file_size = lseek(enroll_file_fd,0,SEEK_END);

        if (file_size == -1) {
            perror("Error getting file size");
            lock.l_type = F_UNLCK;
            fcntl(enroll_file_fd,F_SETLK,&lock);
            close(enroll_file_fd);
            return -2;
        }
        for (int i = file_size - struct_size; i >= 0; i -= struct_size) {
            lseek(enroll_file_fd, i, SEEK_SET);  // Seek to the current position
            if(unEnrollNum > 0){
                struct enroll unEnroll;
                if (read(enroll_file_fd, &unEnroll, struct_size) == -1) {
                    perror("Error reading the file");
                    lock.l_type = F_UNLCK;
                    fcntl(enroll_file_fd ,F_SETLK ,&lock);
                    close(enroll_file_fd);
                    return 1;
                }
                struct enroll w = unEnroll;
                if(w.course_id == cid){
                    int eid = w.enroll_id;
                    w.is_enrolled = 0;
                    lseek(enroll_file_fd ,(eid-1)*sizeof(struct enroll) ,SEEK_SET);
                    write(enroll_file_fd ,&w ,sizeof(struct enroll));
                }
                unEnrollNum--;
            }
            else break;
        }

        lock.l_type = F_UNLCK;
        fcntl(enroll_file_fd,F_SETLK,&lock);
        close(enroll_file_fd);
        return -available;
        }
    }
}

void remove_course_from_catalog(int desc,int id) {
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024];
    int courses_file_fd = open("./courses",O_RDWR);
    if (courses_file_fd == -1) {
        exit(1);
    }
    int courseCount;
    int *cids = view_offering_courses(desc,id,&courseCount,1);
    int enrollCnt;
    struct enroll *e = view_enrollments(desc, id,&enrollCnt ,0);
    int cid;
    while(1){
        write_size = write(desc,"Enter the course ID number to Deactivate: ",
        sizeof("Enter the course ID number to Deactivate: "));
        if(write_size == -1) {
            perror("ERROR IN ASKING COURSE ID TO CLIENT");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc,read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror("ERROR IN READING FROM CLIENT");
            _exit(0);
        }
        if(read_buff[0] == '^'){
            continue;
        }
        if(isalpha(read_buff)){
                write_size = write(desc,"Wrong id entered, Try again.^",
                sizeof("Wrong id entered, Try again.^"));
        }
        else{
            cid = atoi(read_buff);
            if(cid<=0 || get_count(3)<cid){
                write(desc,"Wrong id entered, Try again.^",sizeof("Wrong id entered, Try again.^"));
            }
            else
                break;
        }
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < courseCount; i++) {
        if (cids[i] == cid) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        write(desc,"Course not Found.^",sizeof("Course not Found.^"));
        return ;
    }
    struct flock lock = {F_WRLCK, SEEK_SET, (id-1)*sizeof(struct course), sizeof(struct course), getpid()};
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = (id-1)*sizeof(struct course);
    // lock.l_len = sizeof(struct course);
    // lock.l_pid = getpid();
    int locking_stat = fcntl(courses_file_fd, F_SETLKW, &lock);
    if(locking_stat == -1){
        perror("Error obtaining write lock on Course Record!");
        close(courses_file_fd);
    }
    lseek(courses_file_fd,(id-1)*sizeof(struct course),SEEK_SET);
    struct course c;
    read(courses_file_fd ,&c ,sizeof(struct course));
    //c.isActive = 0;
    lseek(courses_file_fd,(id-1) * sizeof(struct course) ,SEEK_SET);
    write(courses_file_fd , &c, sizeof(struct course));
    lock.l_type = F_UNLCK;
    fcntl(courses_file_fd ,F_SETLK ,&lock);
    close(courses_file_fd);
    int enroll_file_fd = open("./enroll",O_RDWR);
    if (enroll_file_fd == -1) {
        exit(1);
    }
    // Un-enrolling students from deleted course
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    locking_stat = fcntl(enroll_file_fd, F_SETLKW, &lock);
    if(locking_stat==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(enroll_file_fd);
    }
    lseek(enroll_file_fd,0,SEEK_SET);    
    for(int i=0;i<enrollCnt;i++){
        struct enroll w = e[i];
        if(w.course_id == id){
            int eid = w.enroll_id;
            w.is_enrolled = 0;
            lseek(enroll_file_fd,(eid-1)*sizeof(struct enroll),SEEK_SET);
            write(enroll_file_fd,&w,sizeof(struct enroll));
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(enroll_file_fd,F_SETLK,&lock);
    close(enroll_file_fd);
}

void update_course_details(int desc, int id) {
    int courseCount;
    int *cids = view_offering_courses(desc,id,&courseCount,1);
    int cid;
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    while(1){
        write_size = write(desc,"Enter the course ID number to Modify: ",
        sizeof("Enter the course ID number to Modify: "));
        if(write_size == -1) {
            perror("ERROR IN ASKING COURSE ID TO CLIENT");
            _exit(0);
        }
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc,read_buff,sizeof(read_buff));
        if(read_buff[0] == '^') 
            continue;
        id = atoi(read_buff);
        if(id <= 0 || get_count(3) < id){
            write(desc,"Wrong id entered.^",sizeof("Wrong id entered.^"));
        }
        else
            break;
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < courseCount; i++) {
        if (cids[i] == id) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        write(desc ,"Course not Found.^" ,sizeof("Course not Found.^"));
        return;
    }
    int courses_file_fd = open("./courses",O_RDWR);
    if (courses_file_fd == -1) {
        exit(1);
    }
    struct flock lock = {F_WRLCK, SEEK_SET, (id-1) * sizeof(struct course), sizeof(struct course), getpid()};
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = (id-1)*sizeof(struct course);
    // lock.l_len = sizeof(struct course);
    // lock.l_pid = getpid();
    int locking_stat = fcntl(courses_file_fd, F_SETLKW, &lock);
    if(locking_stat==-1){
        perror("Error obtaining write lock on Course Record!");
        close(courses_file_fd);
    }
    lseek(courses_file_fd ,(id-1) * sizeof(struct course) ,SEEK_SET);
    struct course c;
    read(courses_file_fd ,&c ,sizeof(struct course));
    int choice;
    while(1){
        write(desc,"Enter option{1. Course Name, 2. Credits, 3. Seats}: ",
        sizeof("Enter option{1. Course Name, 2. Credits, 3. Seats}: "));
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if(read_buff[0] == '^'){
            continue;
        }
        choice = atoi(read_buff);
        if(choice < 1 || choice > 3 ){
            write(desc,"Wrong option entered.^",sizeof("Wrong option entered.^"));
        }
        else
            break;
    }
    char credits[4];
    switch (choice)
        {
        case 1:
            write_size = write(desc,"Enter Course Name: ",sizeof("Enter Course Name: "));
            if(write_size == -1) {
                perror("ERROR IN ASKING COURSE ID TO CLIENT");
                _exit(0);
            }
            read_size = read(desc,read_buff,sizeof(read_buff));
            if(read_size == -1) {
                perror("ERROR IN READING COURSE ID FROM CLIENT");
                _exit(0);
            }
            strcpy(c.course_name, read_buff);
            break;
            
        case 2: 
            write_size = write(desc,"Enter Credits: ",sizeof("Enter Credits: "));
            if(write_size == -1) {
                perror("ERROR IN ASKING CREDIT TO CLIENT");
                _exit(0);
            }
            bzero(read_buff, sizeof(read_buff));
            read_size = read(desc,read_buff,sizeof(read_buff));
            if(read_size == -1) {
                perror("ERROR IN READING CREDIT FRO  CLIENT");
                _exit(0);
            }
            c.credit = atoi(read_buff);
            break;
        case 3:
            write_size = write(desc,"Enter Seats: ",sizeof("Enter Seats: "));
            if(write_size == -1) {
                perror("ERROR IN ASKING SEATS TO CLIENT");
                _exit(0);
            }
            bzero(read_buff, sizeof(read_buff));
            read_size = read(desc,read_buff, sizeof(read_buff));
            int seats = atoi(read_buff);
            int retV = handle_seats(desc,id,cid,c.seats,seats,c.available);
            c.available +=  retV;
            c.seats = seats;
            break;
        default:
            break;
        }
        lseek(courses_file_fd,(id-1)*sizeof(struct course),SEEK_SET);
        write(courses_file_fd,&c,sizeof(struct course));

        lock.l_type = F_UNLCK;
        fcntl(courses_file_fd,F_SETLK,&lock);
        close(courses_file_fd);
}

void change_password_professor(int desc, int id) {}

void prof_login_handler (int desc) {
    struct prof professor;
    struct stud student;
    ssize_t write_size, read_size;
    char read_buff[1024], write_buff[1024];
    bzero(write_buff, 1024);
    bzero(read_buff, 1024);
    write_size = write(desc, ASK_LOGIN, sizeof(ASK_LOGIN));
    if(write_size == -1) {
        perror("CANNOT ASK LOGIN ID");
        _exit(0);
    }
    read_size = read(desc, read_buff, sizeof(read_buff));
    if(read_size == -1) {
        perror("CANNOT READ LOGIN ID");
        _exit(0);
    }
    int id = atoi(read_buff); 
    if(login(false, false, desc, id)) {
        strcpy(write_buff, LOGIN_SUCCESS);
        while(1) {
            write_size = write(desc, PROF_MENU, sizeof(PROF_MENU));
            if(write_size == -1) {
                perror("COULDN'T WRITE");
                _exit(0);
            } 
            bzero(write_buff, 1024);
            read_size = read(desc, read_buff, sizeof(read_buff));
            if(read_size == -1) {
                perror("COULDN'T READ");
                _exit(0);
            }
            int ch = atoi(read_buff);
            printf("%d", ch);
            switch(ch) {
                case 1:
                    int course_cnt;
                    view_offering_courses(desc, id, &course_cnt, 1);
                    break;
                case 2:
                    add_new_course(desc, id);
                    break;
                case 3:
                    remove_course_from_catalog(desc, id);
                    break;
                case 4:
                    update_course_details(desc, id);
                    break;
                case 5:
                    change_password_professor(desc, id);
                    break;
                default:
                    write(desc, LOGOUT_MSG, sizeof(LOGOUT_MSG));
                    exit(EXIT_SUCCESS);
            }
        }
    }
}