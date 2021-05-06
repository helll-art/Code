test : main.o Manager.o Admin.o Account.o Student.o Appointment.o Teacher.o
	g++ -o test main.o Manager.o Admin.o Account.o Student.o Appointment.o Teacher.o
Appointment.o : Appointment.cpp Appointment.h
	g++ -c Appointment.cpp
Teacher.o : Teacher.cpp Teacher.h
	g++ -c Teacher.cpp
Student.o : Student.cpp	 Student.h
	g++ -c Student.cpp
Admin.o : Admin.cpp
	g++ -c Admin.cpp
Account.o : Account.cpp	
	g++ -c Account.cpp
Manager.o : Manager.cpp
	g++ -c Manager.cpp
main.o : main.cpp
	g++ -c main.cpp
clear : 
	rm *.o