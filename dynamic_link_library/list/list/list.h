#ifdef LIST_EXPORTS
#define LIST_API __declspec(dllexport)
#else
#define LIST_API __declspec(dllimport)
#endif

struct Node {
	int element;
	Node* nextNode = nullptr;
};

struct sList {
	Node* head;
};

class LIST_API List {
private: 
	sList list{ nullptr };

public:
	List();
	List(int element);

	~List();

	void add(int element);
	void del(int position);
	void insert(int position, int element);
	int size();
};
