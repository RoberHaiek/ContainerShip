roberhaiek 205962657

using namespace shipping;

template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class X: public Named<int> {
    using Named<int>::Named;
};

class Y: public Named<int> {
    using Named<int>::Named;
};

class Height: public Named<int> {
    using Named<int>::Named;
};

// a grouping unordered map, key is string, value is function that returns string: f(container) = container's_group (grouping can be according to destPort, owner, weight, etc...)
template<typename Container>
using Grouping = std::unordered_map<std::string, std::function<string(const Container&)>>;

template<typename Container>
class Ship {

	X x;
	Y y;
	Container*** containers;
	std::map<std::pair<X,Y>,std::pair<int,int>> rowColumn_maxheightSize;

	void handleRestrictions(std::vector<std::tuple<X, Y, Height>> restrictions){
		for(int i=0;i<restrictions.size();i++){
			rowColumn_maxheightSize->insert_or_assign(std::pair<X,Y>(std::get<0>(restrictions[i]),std::get<1>(restrictions[i])),std::pair<Height,int>(std::get<2>(restrictions[i]),0));
		}
	}

	Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions,
			Grouping<Container> groupingFunctions) noexcept(false){
		Ship(x,y,max_height,restrictions);

	}

	Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false){
		Ship(x,y,max_height);
		handleRestrictions(restrictions);
	}

	Ship(X x, Y y, Height max_height) noexcept{
		containers = new Container**[x];
		for (int i=0;i<x;i++){
			containers[i] = new Container*[y];
			for(int j=0;j<y;j++){
				rowColumn_maxheightSize->insert(std::pair<X,Y>(i,j),std::pair<Height,int>(max_height,0));
				containers[i][j] = new Container[max_height];
			}
		}
	}

	// these 3 methods may return BadShipOperationException
	void load(X x, Y y, Container c) noexcept(false){
		int size = (rowColumn_maxheightSize->find(std::pair<X,Y>(x,y))).second+1;
		int max_height = (rowColumn_maxheightSize->find(std::pair<X,Y>(x,y))).first;
		containers[x][y][size-1] = c;
		rowColumn_maxheightSize->insert_or_assign(std::pair<X,Y>(x,y),std::pair<Height,int>(max_height,size));
	}

	Container unload(X x, Y y) noexcept(false){ 
		int size = (rowColumn_maxheightSize->find(std::pair<X,Y>(x,y))).second-1;
		int max_height = (rowColumn_maxheightSize->find(std::pair<X,Y>(x,y))).first;
		rowColumn_maxheightSize->insert_or_assign(std::pair<X,Y>(x,y),std::pair<Height,int>(max_height,size));
		return containers[x][y][size];
	}

	void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false){
		load(to_x,to_y,unload(from_x,from_y));
	}

	getContainersViewByPosition(X x, Y y) const;

	getContainersViewByGroup(const string& groupingName, const string& groupName) const;

	BadShipOperationException(string msg);

	Container operator++(){ c++; return containers[c]; }

}