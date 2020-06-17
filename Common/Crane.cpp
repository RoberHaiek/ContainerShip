#include "CraneTester.cpp"

class Crane{
public:
	Ship* ship;
	Crane(Ship* ship):ship(ship){
	}
	// unload a single container from a specific location
	node * unload(Container container, int row, int column, int floor) {
		
		struct node *temp,*prev,*beforeEmptyContainers;
		temp=ship->planLinkedList[row][column].linkedList;
		int emptyContainerIndecator=0;		
		prev=temp;
		beforeEmptyContainers=temp;
		while(temp!=NULL && temp->container->uniqueId.compare(container.uniqueId)!=0 ){
			
			prev=temp;
			temp=temp->next;
			if(temp!=NULL && temp->container->uniqueId=="empty"){
				if(!emptyContainerIndecator){
					emptyContainerIndecator=1;
				}	beforeEmptyContainers=prev;
			}else if(temp!=NULL && temp->next!=NULL && temp->next->container->uniqueId!="empty"){
				emptyContainerIndecator=0;
			}
		}
		if(temp==prev){
			if(temp->next==NULL){
			node *newNode =new node();
			ship->planLinkedList[row][column].linkedList=newNode;
			}else{
				ship->planLinkedList[row][column].linkedList=temp->next;
			}
		}else{
			prev->next=temp->next;
		}
			ship->planMap->erase(container.uniqueId);
			//ship->contMap->erase(container.uniqueId);
			//ship->planLinkedList[row][column].size--;
		if(floor<-1){
			return temp;
		}
		int numOfRemovedEmpty=0;
		if(emptyContainerIndecator){//remove all the empty containers
			struct node *temp2;
			temp2=beforeEmptyContainers;
			while(temp2->next!=NULL){
				numOfRemovedEmpty++;
				temp2=temp->next;
			}
			temp=beforeEmptyContainers;
			ship->planLinkedList[row][column].size-=numOfRemovedEmpty;
		}	
		return temp;
	}

	// load a single container to a specific location
	void load(Container* container,int row, int column, int floor) {
		int* rowColumn = new int[3];
		rowColumn[0] = row;
		rowColumn[1] = column;
		rowColumn[2] = floor;
		int numOfemptyIndicator=0;
//cout<<"		+-+-+- loading "<<container->uniqueId << " in "<<floor<< ","<<row<<","<<column<<endl; 
		ship->planMap->insert(std::pair<std::string, int*>(container->uniqueId,rowColumn));	// Adding container to the map
		struct node *temp, *newNode;
		newNode =new node();

		Port port = Port(container->destPort.toString());
		newNode->container=new Container(container->weight,port,container->uniqueId);
		newNode->next=NULL;
		temp=ship->planLinkedList[row][column].linkedList;

		while(temp!=NULL && temp->next!=NULL){
			temp=temp->next;
		}
		if(temp==NULL){
			ship->planLinkedList[row][column].linkedList=newNode;
		}

		else if(temp->container->uniqueId.compare("Floor")==0){

			int size=ship->planLinkedList[row][column].size;
			struct node *emptyNode=new node();
			struct node *firstEmptyNode=emptyNode;
			while(floor>size+1){//adding empty containers
				struct node *newEmptyNode=new node();
				numOfemptyIndicator+=1;
				newEmptyNode->container=new Container(0,Port(""),"empty");
				emptyNode->next=newEmptyNode;
				emptyNode=newEmptyNode;
				
				}
			if(numOfemptyIndicator>0){
				emptyNode->next=newNode;
				ship->planLinkedList[row][column].linkedList=firstEmptyNode;	
			}else{
				
				ship->planLinkedList[row][column].linkedList=newNode;//
			}
		}else{
			int size=ship->planLinkedList[row][column].size;
			
			while(floor>size+1){//adding empty containers
				struct node *newEmptyNode=new node();
				numOfemptyIndicator+=1;
				newEmptyNode->container=new Container(0,Port(""),"empty");
				temp->next=newEmptyNode;
				temp=newEmptyNode;
				
				}	
				temp->next=newNode;
			
		}
		ship->planLinkedList[row][column].size+=1+numOfemptyIndicator;
	}

};

