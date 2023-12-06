/* 센서마다 Table 생성*/
-- 온도 센서 
create table temperature(
	id int auto_increment primary key,
    value int,
	create_time timestamp default current_timestamp
);
-- 조도
create table bright(
	id int auto_increment primary key,
    value int,
	create_time timestamp default current_timestamp
);
-- 일산화탄소
create table gas(
	id int auto_increment primary key,
    value int,
	create_time timestamp default current_timestamp
);
-- 적외선 
create table infrared(
	id int auto_increment primary key,
    value boolean,
	create_time timestamp default current_timestamp
);
-- led 값 
create table led(
	id int auto_increment primary key,
    value varchar(20),
    update_time timestamp default current_timestamp
);
-- 부저  
create table buzzer(
	id int auto_increment primary key,
    value boolean,
    update_time timestamp default current_timestamp
);
-- 화재감지
create table is_fire(
	id int auto_increment primary key,
    value varchar(20),
    update_time timestamp default current_timestamp
);

