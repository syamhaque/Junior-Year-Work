#include "p1_process.h"

void get_statistics(std::string class_name[], int num_processes, int num_threads) {

	// You need to create multiple processes here (each process
	// handles at least one file.)
	
	
	// Each process should use the sort function which you have defined  		
	// in the p1_threads.cpp for multithread sorting of the data. 
	
	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.

	vector<Student> student;

	int i = 0, num = 0, count = 0;
	pid_t parent = getpid();

	for(i = 0; i < num_processes; i++){
		if(getpid() == parent){
			fork();
			num = ++count;
		}
	}

	if(getpid() == parent){
		num = 0;
	}
	else	printf("Child process is created. (pid: %d)\n", getpid());

	i = num_processes;
	if(i >= 5){
		i = 5;
		for(i; i >= 1; i--){
			if(num == i){
				student.clear();
				readFile(student, class_name, num-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, num-1);
				outputStats(student, class_name, num-1);
			}
		}
	}
	else{
		for(i; i >= 1; i--){
			if(num == i){
				student.clear();
				readFile(student, class_name, num-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, num-1);
				outputStats(student, class_name, num-1);
			}
		}
		if(num_processes == 1){
			for(i = 1; i <= 5; i++){
				student.clear();
				readFile(student, class_name, i-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, i-1);
				outputStats(student, class_name, i-1);
			}
		}
		else if(num_processes == 2){
			for(i = 2; i <= 5; i++){
				student.clear();
				readFile(student, class_name, i-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, i-1);
				outputStats(student, class_name, i-1);
			}
		}
		else if(num_processes == 3){
			for(i = 3; i <= 5; i++){
				student.clear();
				readFile(student, class_name, i-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, i-1);
				outputStats(student, class_name, i-1);
			}
		}
		else if(num_processes == 4){
			for(i = 4; i <= 5; i++){
				student.clear();
				readFile(student, class_name, i-1);
				threadSort(student, num_threads);
				outputSorted(student, class_name, i-1);
				outputStats(student, class_name, i-1);
			}
		}
	}

	for(i = 0; i < num_processes; i++) wait(NULL);
}

void readFile(vector<Student> &student, string class_name[], int num){

	int i = 0;
	string dir;
	string top;
	double id;
	char comma;
	double grade;
	vector<double> ids;
	vector<double> grades;
	Student s;
	ifstream inFile;

	dir.append("input/").append(class_name[num].c_str()).append(".csv");
	inFile.open(dir.c_str());
	dir.clear();
	if(inFile.is_open()){
		getline(inFile,top);
		while(!inFile.eof()){
			inFile >> id >> comma >> grade;
			ids.push_back(id);
			grades.push_back(grade);
			s.id = id;
			s.grade = grade;
			student.push_back(s);
			i++;
		}
	}
	else	printf("error opening file\n");
	inFile.close();
	student.pop_back();

}

void outputSorted(vector<Student> &student, string class_name[], int num){

	int i = 0;
	string dir;
	string top;
	ofstream outSortedFile;

	dir.append("output/").append(class_name[num].c_str()).append("_sorted.csv");
	outSortedFile.open(dir.c_str());
	dir.clear();
	if(outSortedFile.is_open()){
		outSortedFile << "Rank,Student ID,Grade\n";
		for(i = 0; i < student.size(); i++){
			outSortedFile << i+1 << ',';
			outSortedFile << setprecision(10) << student[i].id << ',';
			outSortedFile << setprecision(12) << student[i].grade << "\n";
		}
	}
	else	printf("error opening file\n");
	outSortedFile.close();

}

void outputStats(vector<Student> &student, string class_name[], int num){

	int i = 0;
	double sum, average, median, var, stdev;
	string dir;
	string top;
	ofstream outStatsFile;

	//calculate average
	for(i = 0; i < student.size(); i++)	sum += student[i].grade;
	average = sum / student.size();

	//calculate median
	if(student.size() % 2 == 0)	median = (student[student.size()/2].grade + student[(student.size()/2)-1].grade)/2;
	else	median = student[student.size()/2].grade;

	//calculate stdev
	for(i = 0; i < student.size(); i++)	var += pow(student[i].grade - average, 2);
	stdev = sqrt(var/student.size());

	dir.append("output/").append(class_name[num].c_str()).append("_stats.csv");
	outStatsFile.open(dir.c_str());
	dir.clear();
	if(outStatsFile.is_open()){
		outStatsFile << "Average,Median,Std. Dev\n";
		outStatsFile << fixed << setprecision(3) << average << ',';
		outStatsFile << fixed << setprecision(3) << median << ',';
		outStatsFile << fixed << setprecision(3) << stdev << "\n";
	}else	printf("error opening file\n");
	outStatsFile.close();

}

void threadSort(vector<Student> &student, int num_threads){

	int i = 0, l = 0, m, r;
	pthread_t threads[num_threads];
	threadData *td;

	for(i = 0; i < num_threads; i++){
		td = (threadData*)malloc(sizeof(threadData));
		td->part = i;
		td->threads = num_threads;
		td->size = (double)student.size();
		td->student = &student;
		pthread_create(&threads[i], NULL, merge_sort, td);
	}

	for(i = 0; i < num_threads; i++)	pthread_join(threads[i], NULL);

	for(i = 0; i < num_threads - 1; i++){
		m = (i + 1) * ((double)student.size() / num_threads) - 1;
		r = (i + 2) * ((double)student.size() / num_threads) - 1;
		merge(student, l, m, r);
	}
}