#include "tinbes.h"

#define TABLE_SIZE 25
#define RUNNING 'r'
#define PAUSED 'p'

string processName[TABLE_SIZE];
int id[TABLE_SIZE];
char state[TABLE_SIZE];
long addr[TABLE_SIZE];
int noOfProcesses = 0;
int idCounter = 0;

int newProcess(string name, long address) { // Maakt een nieuw proces aan
  // Zet process in arrays
  processName[idCounter] = name;
  id[idCounter] = idCounter;
  state[idCounter] = RUNNING;
  addr[idCounter] = address;
  // Processen en id counter wordt ge-increment om voor te bereiden op volgend proces
  noOfProcesses++;
  return idCounter++;
}

void removeProcess(int i) {
  noOfProcesses--;
  for (int j = i; j < noOfProcesses; j++) {
    // Verwijderd proces i en sluit tabel weer aan
    processName[i] = processName[j+1];
    id[i] = id[j+1];
    state[i] = state[j+1];
    addr[i] = address[j+1];
  }
}

void executeProcesses() {
  long newAddr;
  for (int i = 0; i < noOfProcesses; i++) {
    if (state[i] == RUNNING) {
      // Elk proces wat op RUNNING staat wordt uitgevoerd (execute())
      newAddr = execute(addr[i]);

      if (newAddr == 0) {
        cout << "Process \"" << processName[i] << "\" has been terminated." << endl;
        removeProcess(i--);
      } else {
        addr[i] = newAddr;
      }
    }
  }
}

void listProcesses() {
  // Geeft alle processen weer in de output
  for (int i = 0; i < noOfProcesses; i++) {
    cout << id[i] << ' ' << processName[i] << ' ' << state[i] << endl;
  }
}

int findProcess(int i) {
  // Als het process bestaat, geef dan het regelnummer terug in de output
  for (int j = 0; j < noOfProcesses; j++) {
    if (id[j] == i) {
        return j;
    }
  }
  // Als het process niet bestaat, geef dan -1 terug in de output
  return -1;
}

void suspendProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Error: Process does not exist." << endl;
    return;
  }
  if (state[j] == PAUSED) {
    cout << "Error: Process already paused." << endl;
  }
  else {
    // Zet het process op PAUSED en laat dit weten in de output
    state[j] = PAUSED;
    cout << "Process succesfully suspended." << endl;
  }
}

void resumeProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Error: Process does not exist." << endl;
    return;
  }
  if (state[j] == RUNNING) {
    cout << "Error: Process already running." << endl;
  }
  else {
    // Zet het process op RUNNING en laat dit weten in de output
    state[j] = RUNNING;
    cout << "Process succesfully resumed." << endl;
  }
}

void killProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Error: Process does not exist." << endl;
  } else {
    // Verwijder process uit tabel removeProcess()) en laat dit weten in de output
    removeProcess(j);
    cout << "Process succesfully killed." << endl;
  }
}

int main() {
  unblockCin();
  string command;
  while (true) {
    if (charactersAvailable()) { // Checkt of er data input aanwezig is
      command = readLine();
      if (command == "RUN") {
        string name = readLine();
        long address = stoi(readLine());
        cout << newProcess(name, address) << endl;
      }
      if (command == "LIST") {
        listProcesses();
      }
      if (command == "SUSPEND") {
        int i = stoi(readLine());
        suspendProcess(i);
      }
      if (command == "RESUME") {
        int i = stoi(readLine());
        resumeProcess(i);
      }
      if (command == "KILL") {
        int i = stoi(readLine());
        killProcess(i);
      }
    } else {
      executeProcesses();
    }
  }
}
