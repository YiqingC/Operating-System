#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<queue>


using namespace std;

const int MAINMEMORYSIZE = 1048576; // 1*1024*1024

struct Process{
	char op;
	int pid;
	int size;
	int begin;
	int end;
};

struct Free{
	int pid;
	int begin;
	int end;
	int size;
};

void ReadInFile(string, vector<Process>&, int&);
int FindPid(int, vector<Process>&);
void ProcessJob(int, int, vector<Process>&, int, vector<Free>&, vector<Free>&);

int main (){
	string FileName;
	vector<Process> incoming;
	int TotalNumOfJob = 0;
	int TotalMemoryUsed = 0;

	vector<Free> FreeBlocks;
	vector<Free> JobLeft;

	cout << "Please enter the file name: " << endl;
	// cin >> FileName;
	FileName = "inputOS4.txt";
	ReadInFile(FileName, incoming, TotalNumOfJob);


	// // test read file in
	// for(int i = 0; i < 31; i++)
	// {
	// 	cout << incoming[i].op << "\t" << incoming[i].pid << "\t" << incoming[i].size << "\n";
	// }

	int VectorSize;
	VectorSize = incoming.size();
	ProcessJob(TotalNumOfJob, TotalMemoryUsed, incoming, VectorSize, FreeBlocks, JobLeft);

	int newsize = JobLeft.size();
	for(int i = 0; i < newsize; i++){
		cout << JobLeft[i].pid << "\t";
	}
	cout << endl;


	return 0;
}


void ReadInFile(string FileName, vector<Process>& incoming, int& TotalNumOfJob){
	ifstream fin;
	fin.open(FileName.c_str());
	if(fin.is_open()){ // check if file is open
		Process NewProcess;
		while(fin){
			fin >> NewProcess.op;
			if(NewProcess.op != 'Q'){
				if(NewProcess.op != 'D'){
					fin >> NewProcess.pid >> NewProcess.size;
					TotalNumOfJob++;
				}
				else{
					fin >> NewProcess.pid;
					NewProcess.size = 0;
				}
			}
			else{	
				NewProcess.pid = 0;
				NewProcess.size = 0;
			}
			incoming.push_back(NewProcess);
		}
	}
	else{
		cout << "No such file exists!" << endl;
	}
}

int FindPid(int Pid, vector<Process>& incoming){
	bool found = 0;
	int index = -1;
	while(!found){
		index++;
		if(Pid == incoming[index].pid){
			found = 1;
		}
	}
	if(found == 1){
		return index;
	}
	else{
		return -1;
	}
}

void ProcessJob(int TotalNumOfJob, int TotalMemoryUsed, vector<Process>& incoming, int VectorSize, vector<Free>& FreeBlocks, vector<Free>& JobLeft){
	int NumOfJobLeft = 0;
	int index;
	Free NewFreeBlock;

	int NumOfFreeBlocks = 0;
	int j = 0;

	int LatestEndPosition = 0;
	int NumOfD = 0;
	for(int i = 0; i < VectorSize; i++){

		if(incoming[i].op == 'A'){
			if(incoming[i].size <= MAINMEMORYSIZE){
				if(TotalMemoryUsed + incoming[i].size <= MAINMEMORYSIZE){
					if(!FreeBlocks.empty()){
						NumOfFreeBlocks = FreeBlocks.size();
						j = 0;
						cout << "before while " << NumOfFreeBlocks << "\t" << j << "\n";
						while(FreeBlocks[j].size < incoming[i].size && j < NumOfFreeBlocks){

							cout << "search for next free block\n";

							j++;
							cout << "in while " << j << "\n";
						}
						cout << "after while " << NumOfFreeBlocks << "\t" << j << "\n";



					}
					if(j != NumOfFreeBlocks){
						cout << "put in this free block\n";
						incoming[i].begin = FreeBlocks[j].begin;
						TotalMemoryUsed += incoming[i].size;
						incoming[i].end = incoming[i].begin + incoming[i].size - 1;

						FreeBlocks[j].begin += incoming[i].size;
						// FreeBlocks[j].end   Stay the same?
						FreeBlocks[j].size -= incoming[i].size;

						cout << incoming[i].pid << "\t" 
						<< incoming[i].begin << "\t" 
						<< incoming[i].end << "\t" 
						<< incoming[i].size << "\n";
						cout << "New Free" << "\t" 
						<< FreeBlocks[j].begin << "\t" 
						<< FreeBlocks[j].end << "\t" 
						<< FreeBlocks[j].size << "\n";

						cout << "TotalMemoryUsed now is " << TotalMemoryUsed << "\n";

					}
					else if(!FreeBlocks.empty() && j == NumOfFreeBlocks && TotalMemoryUsed < LatestEndPosition){
						cout << "free block is smaller than size, put at the end of all\n";
						incoming[i].begin = LatestEndPosition;
						TotalMemoryUsed += incoming[i].size;
						LatestEndPosition += incoming[i].size;
						incoming[i].end = incoming[i].begin + incoming[i].size - 1;

						cout << incoming[i].pid << "\t" 
						<< incoming[i].begin << "\t" 
						<< incoming[i].end << "\t" 
						<< incoming[i].size << "\n";

						cout << "TotalMemoryUsed now is " << TotalMemoryUsed << "\n";
					}
					else{
						incoming[i].begin = TotalMemoryUsed;
						TotalMemoryUsed += incoming[i].size;
						incoming[i].end = TotalMemoryUsed - 1;
						LatestEndPosition += incoming[i].size;

						cout << incoming[i].pid << "\t" 
						<< incoming[i].begin << "\t" 
						<< incoming[i].end << "\t" 
						<< incoming[i].size << "\n";
						cout << "TotalMemoryUsed now is " << TotalMemoryUsed << "\n";
						cout << "LatestEndPosition now is " << LatestEndPosition << "\n";
					}

				


				}
				else{
					JobLeft[NumOfJobLeft].pid = incoming[i].pid;
					JobLeft[NumOfJobLeft].begin = 0;
					JobLeft[NumOfJobLeft].end = 0;
					JobLeft[NumOfJobLeft].size = 0;
					NumOfJobLeft++;
				}
			}
			else{
				JobLeft[NumOfJobLeft].pid = incoming[i].pid;
				JobLeft[NumOfJobLeft].begin = 0;
				JobLeft[NumOfJobLeft].end = 0;
				JobLeft[NumOfJobLeft].size = 0;
				NumOfJobLeft++;
			}
		}
		else if(incoming[i].op == 'D'){
			
			//newadd
			for(int k = 0; k < incoming[i].pid; k++){
				if(incoming[k].op == 'D'){
					NumOfD++;
				}
			}

			cout << "NumOfJobLeft now is " << NumOfJobLeft << "\n";
			



			index = FindPid(incoming[i].pid, incoming);
			// index -= NumOfD;//newadd
			if(index != -1){
				cout << "index is " << index << "\n";
				NewFreeBlock.pid = index + 1 - NumOfD;
				NewFreeBlock.begin = incoming[index].begin;
				NewFreeBlock.end = incoming[index].end;
				NewFreeBlock.size = incoming[index].size;
				TotalMemoryUsed -= incoming[index].size;
				incoming[index].size = 0;
			

				if(!FreeBlocks.empty()){
					NumOfFreeBlocks = FreeBlocks.size();
					int j = 0;
					while(index + 1 - NumOfD > FreeBlocks[j].pid && j < NumOfFreeBlocks && NewFreeBlock.begin > FreeBlocks[j].begin){
						j++;
					}
					FreeBlocks.insert(FreeBlocks.begin() + j, NewFreeBlock);
				}
				else{
					FreeBlocks.push_back(NewFreeBlock);
				}

				// FreeBlocks.push_back(NewFreeBlock);
				// FreeBlocks.insert(FreeBlocks.begin(), NewFreeBlock);

				cout << "Free\t" 
				<< NewFreeBlock.pid << "\t"
				<< NewFreeBlock.begin << "\t"
				<< NewFreeBlock.end << "\t"
				<< NewFreeBlock.size << "\n";

				cout << "TotalMemoryUsed now is " << TotalMemoryUsed << "\n";

				int newsize = FreeBlocks.size();
				for(int i = 0; i < newsize; i++)
				{
					cout << FreeBlocks[i].begin << "\t" 
					<< FreeBlocks[i].end << "\t" 
					<< FreeBlocks[i].size << "\n";
				}


			}
		}


	}


}







