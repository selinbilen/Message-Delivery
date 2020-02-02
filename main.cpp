/* @Author
   Student Name: Selin Eylul Bilen
   Date: 21.12.2019
   to compile: g++ -Wall -Werror mian.cpp -o main
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define MAX_LINE_LENGTH 128

using namespace std;

struct Message {
  char* text;
  int hostId;
};

struct MessageNode {
  Message data;
  MessageNode* next;
};

struct PathNode {
  int id;
  PathNode* next;
};

struct Path {
  PathNode* head;
  void create();
  void destroy();
  void push(int);
  int pop();
  bool isempty();
};

struct MobileHost {
  int id;
  MobileHost* next;
};

struct BaseStation {
  int id;
  BaseStation* children;
  BaseStation* nextSibling;
  MobileHost* mobileHosts;
};

struct Network {
  BaseStation* centralController;
  void create(char*);
  void destroy();
  void emptyNetwork(BaseStation*);
  void addBS(BaseStation*, int);
  void addMH(MobileHost*, int);
  BaseStation* searchBS(BaseStation*, int);
  int searchMH(BaseStation*, int, Path*);
  void read_fromfile(char*);
  bool isempty();
};

struct MessageQueue {
  MessageNode* front;
  MessageNode* back;
  void create(char*);
  void destroy();
  void enqueue(Message);
  Message dequeue();
  void read_fromfile(char*);
  bool isempty();
};

void Network::create(char* filename) {
  centralController = NULL;
  read_fromfile(filename);
}

void Network::destroy() {
  emptyNetwork(centralController);
  delete centralController;
  centralController = NULL;
}

void Network::emptyNetwork(BaseStation* src) {
  if(src == NULL) {
    // Base case 1: if source base station is NULL, return NULL
    return;
  }
  else if(src->children == NULL) {
    // Base case 2: if base station has no children, delete all mobile hosts
    while(src->mobileHosts != NULL) {
      MobileHost* tmpMH = src->mobileHosts;
      src->mobileHosts = src->mobileHosts->next;
      delete tmpMH;
    }
  }
  else {
    // For all the children, perform recursive call and delete them after it
    while(src->children != NULL) {
      BaseStation* tmp = src->children;
      emptyNetwork(tmp);
      src->children = src->children->nextSibling;
      delete tmp;
    }
    // After all the children are deleted, call recursively once again to delete the mobile hosts
    emptyNetwork(src);
  }
}

void Network::addBS(BaseStation* bs, int parent) {
  // First, find the parent base station
  BaseStation* parentBs = searchBS(centralController, parent);
  BaseStation* tmp;
  // If parent base station is present, add a child to it
  if(parentBs != NULL) {
    tmp = parentBs->children;
    if(tmp != NULL) {
      // If there are already some children, append the new one to the end
      while(tmp->nextSibling != NULL) {
        tmp = tmp->nextSibling;
      }
      tmp->nextSibling = bs;
    }
    else {
      // Otherwise, just assign the new child to the head pointer of children list
      parentBs->children = bs;
    }
  }
}

void Network::addMH(MobileHost* mh, int bsId) {
  // First, find the parent base station
  BaseStation* parentBs = searchBS(centralController, bsId);
  MobileHost* tmp;
  // If parent base station is present, add a mobile host to it
  if(parentBs != NULL) {
    tmp = parentBs->mobileHosts;
    if(tmp != NULL) {
      // If there are already some mobile hosts, append the new one to the end
      while(tmp->next != NULL) {
        tmp = tmp->next;
      }
      tmp->next = mh;
    }
    else {
      // Otherwise, just assign the new mobile host to the head pointer of mobile hosts list
      parentBs->mobileHosts = mh;
    }
  }
}

BaseStation* Network::searchBS(BaseStation* src, int bsId) {
  if(src == NULL) {
    // Base case 1: if source base station is NULL, return NULL
    return NULL;
  }
  else if(src->id == bsId) {
    // Base case 2: if base station with id is found, return that base station
    return src;
  }
  else {
    BaseStation* tmp;
    BaseStation* res;
    // For every child, call searchBS recursively and check the result
    for(tmp = src->children; tmp != NULL; tmp = tmp->nextSibling) {
      res = searchBS(tmp, bsId);
      if(res != NULL) {
        // If the base station with id is found, return the pointer to it
        return res;
      }
    }
    // Otherwise, if it was not present in any children base stations, return NULL
    return NULL;
  }
}

int Network::searchMH(BaseStation* src, int mhId, Path* mhPath) {
  cout << src->id << " ";
  if(src == NULL) {
    // Base case 1: if source base station is NULL, return -1
    return -1;
  }
  else {
    // Base case 2: if mobile host with id is found, push the base station id to path stack and return base station id
    MobileHost* tmpMH;
    for(tmpMH = src->mobileHosts; tmpMH != NULL; tmpMH = tmpMH->next) {
      if(tmpMH->id == mhId) {
        mhPath->push(src->id);
        return src->id;
      }
    }

    // For every child, call searchMH recursively and check the result
    BaseStation* tmp;
    for(tmp = src->children; tmp != NULL; tmp = tmp->nextSibling) {
      // If the mobile host with id is found, push the current base station id to stack path and return base station id where it was found
      int res = searchMH(tmp, mhId, mhPath);
      if(res >= 0) {
        mhPath->push(src->id);
        return res;
      }
    }
    // Otherwise, if it was not present in any children base stations, return -1
    return -1;
  }
}

void Network::read_fromfile(char* filename) {
  FILE *network_file;
  // Check if the file can be opened
  if(!(network_file = fopen(filename, "r"))) {
    cerr << "File " << filename << " could not be open." << endl;
    return;
  }
  // Create the central controller node
  centralController = new BaseStation;
  centralController->id = 0;
  centralController->children = NULL;
  centralController->mobileHosts = NULL;
  // Read the network from the file
  char dataType[3];
  int id, parent;
  BaseStation* bs;
  MobileHost* mh;
  while(fscanf(network_file, "%s%d%d", dataType, &id, &parent) == 3) {
    // If it is base station, add it to the network
    if(!strcmp(dataType, "BS")) {
      bs = new BaseStation;
      bs->id = id;
      bs->children = NULL;
      bs->nextSibling = NULL;
      bs->mobileHosts = NULL;
      addBS(bs, parent);
    }
    // If it is a mobile host, add it to the base station
    if(!strcmp(dataType, "MH")) {
      mh = new MobileHost;
      mh->id = id;
      mh->next = NULL;
      addMH(mh, parent);
    }
  }
  // Close the file
  fclose(network_file);
}

void MessageQueue::create(char* filename) {
  front = NULL;
  back = NULL;
  read_fromfile(filename);
}

void MessageQueue::destroy() {
  MessageNode* tmp;
  while(front != NULL) {
    tmp = front;
    front = front->next;
    delete tmp;
  }
}

void MessageQueue::enqueue(Message msg) {
  MessageNode* tmp = new MessageNode;
  tmp->data = msg;
  tmp->next = NULL;
  if(isempty()) {
    // If the queue was empty, set both front and back to point to the new element
    back = tmp;
    front = back;
  }
  else {
    // Otherwise, append it to the end
    back->next = tmp;
    back = tmp;
  }
}

Message MessageQueue::dequeue() {
  MessageNode* top;
  Message tmp;
  // Take the data from the top, delete the element, and return the data
  top = front;
  front = front->next;
  tmp = top->data;
  delete top;
  return tmp;
}

void MessageQueue::read_fromfile(char* filename) {
  FILE *message_file;
  // Check if the file can be opened
  if(!(message_file = fopen(filename, "r"))) {
    cerr << "File " << filename << " could not be open." << endl;
    return;
  }
  // Read the messages from the file
  char line[MAX_LINE_LENGTH];
  char* limitPtr;
  char* tmp;
  while(fgets(line, MAX_LINE_LENGTH, message_file)) {
    Message msg;
    // Find the '>' character
    limitPtr = strchr(line, '>');
    // Replace it with null to divide the string in two parts
    *limitPtr = '\0';
    // First part will be message text, store it to the structure
    msg.text = new char[strlen(line)];
    strcpy(msg.text, line);
    // Second part has to be converted to number
    msg.hostId = strtol(limitPtr+1, &tmp, 10);
    // Store the data to queue
    enqueue(msg);
  }
  // Close the file
  fclose(message_file);
}

bool MessageQueue::isempty() {
  return front == NULL;
}

void Path::create() {
  head=NULL;
}

void Path::destroy() {
  PathNode *p;
  while (head) {
    p = head;
    head = head->next;
    delete p;
  }
}

void Path::push(int newId) {
  PathNode* newNode = new PathNode;
  // Insert the new element at the top
  newNode->id = newId;
  newNode->next=head;
  head=newNode;
}

int Path::pop() {
  PathNode *top;
  int tmp;
  // Take the data from the top, delete the element, and return the data
  top = head;
  head = head->next;
  tmp = top->id;
  delete top;
  return tmp;
}

bool Path::isempty() {
  return head==NULL;
}

int main(int argc, char* argv[]) {
  // Check the number of arguments
  if(argc != 3) {
    cout << "The program must be run as: " << argv[0] << " input1 input2" << endl;
    return 0;
  }

  // Create and fill the network and message queue
  Network ntwrk;
  ntwrk.create(argv[1]);
  MessageQueue msgs;
  msgs.create(argv[2]);
  // Create the stack path to mobile host
  Path mhPath;
  mhPath.create();

  // For each message in queue, try to find the mobile host in network
  while(!msgs.isempty()) {
    Message msg = msgs.dequeue();
    cout << "Traversing:";
    int destId = ntwrk.searchMH(ntwrk.centralController, msg.hostId, &mhPath);
    cout << endl;
    if(destId >= 0) {
      cout << "Message:" << msg.text << " To:";
      while(!mhPath.isempty()) {
        cout << mhPath.pop() << " ";
      }
      cout << "mh_" << msg.hostId << endl;
    }
    else {
      cout << "Can not be reached the mobile host mh_" << msg.hostId << " at the moment" << endl;
    }
    delete[] msg.text;
  }

  // Delete the network, queue and stack path
  ntwrk.destroy();
  msgs.destroy();
  mhPath.destroy();

  return 0;
}
