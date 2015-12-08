#include "HW2BST.h"

using namespace std;

HW2BST::Node::Node(int dataValue)
{
	Data = dataValue;
	Left = Right = NULL;
}

HW2BST::HW2BST(void)
{
	m_root = NULL;
}

HW2BST::~HW2BST(void)
{
	DeleteTree(m_root);
	m_root = NULL;
}

// Should return false if the value already exists in the tree or if memory 
// for a new node could not be allocated.
bool HW2BST::Add(Node* &node, const int &dataValue)
{
	if (!node) {
		node = new Node(dataValue);
		if (node)	// checks if memory was allocated successfully 
			return true;
		else
			return false;
	}
	if (dataValue < node->Data) {				// if less than
		return Add(node->Left, dataValue);
	}
	else if (dataValue > node->Data) {			// if greater than
		return Add(node->Right, dataValue);		// if we keep going right we will add a new level each time making it O(n)
	}
	else
		return false;
}

bool HW2BST::Add(int dataValue)
{
	return Add(m_root, dataValue);	// calls recursive overloaded function Add
}

// Returns the number of levels in the tree. A tree with no contents (null root) will have 
// zero levels. A tree with just the root and no children has 1 level. For all other cases 
// the number of levels must be counted. There is a linear-time algorithm for this.
int HW2BST::CountLevels(Node* root)
{
	int count = 0;
	if (root) {
		count = max(CountLevels(root->Left), CountLevels(root->Right)) + 1;
		return count;
	}
	else
		return 0;
}

int HW2BST::CountLevels()
{
	return CountLevels(m_root);		// calls recursive overloaded function CountLevels
}

void HW2BST::DeleteTree(Node* root)
{
	if (root)
	{
		DeleteTree(root->Left);
		DeleteTree(root->Right);
		delete root;
	}
}

void HW2BST::DisplayContents(Node* node, ostream& outputStream)
{
	if (node)
	{
		DisplayContents(node->Left, outputStream);

		// Display the numerical value for this node
		outputStream << node->Data << " ";

		// Display the entire right subtree
		DisplayContents(node->Right, outputStream);
	}
}

void HW2BST::DisplayContents(ostream& outputStream)
{
	outputStream << "BST Contents: ";
	DisplayContents(m_root, outputStream);
	outputStream << endl;
}

void HW2BST::DisplayLevels(Node* root, std::ostream& outputStream)
{
	if (!root) {
		return;
	}

	Node *currentNode = NULL;
	queue <Node*> currentLevel, nextLevel;				// Create queues for currentLevel and nextLevel of node Pointers

	currentLevel.push(root);

	while (!currentLevel.empty()) {						// traverse queue
		currentNode = currentLevel.front();	
		currentLevel.pop();
		if (currentNode) {
			outputStream << currentNode->Data << " ";
			nextLevel.push(currentNode->Left);			// Load children left to right
			nextLevel.push(currentNode->Right);
		}

		if (currentLevel.empty()) {
			outputStream << endl;
			swap(currentLevel, nextLevel);
		}
	}
}

void HW2BST::DisplayLevels(std::ostream& outputStream)
{
	if (CountLevels())
	{
		outputStream << endl << "Number of levels:" << CountLevels() << endl;
		DisplayLevels(m_root, outputStream);	// recursive step
	}
	else
		outputStream << "The tree is empty" << endl;
}

// Gets the maximum value in the tree. Returns 0 if the tree is empty.
int HW2BST::GetMax()
{
	if (m_root)
	{
		Node* n = m_root;
		while (n->Right)
		{
			n = n->Right;
		}
		return n->Data;
	}
	return 0;
}

int HW2BST::RemoveSmallest(Node* &parent)
{
	while (parent->Left) {
		parent = parent->Left;	// traverse down the left branch until we find the smallest value
	}
	int smallestData = parent->Data;
	DeleteNode(parent);			// remove it
	return smallestData;		// returns the smallestData value
}

bool HW2BST::IsEmpty()
{
	return (NULL == m_root);
}

void HW2BST::DeleteNode(Node* &node)
{
	Node *tmp = node;
	if (node->Left){
		node = node->Left;
	}
	else {						// NULL if there are no children
		node = node->Right;
	}
	delete tmp;
}

// Returns true if the value was found and removed, false if it was not found
bool HW2BST::Remove(Node* &node, const int &dataValue)
{
	if (!node) {
		return false;
	}
	if (dataValue < node->Data)	{			// if less than
		Remove(node->Left, dataValue);
	}
	else if (dataValue > node->Data) { 		// if greater than
		Remove(node->Right, dataValue);
	}
	else if (node->Left && node->Right)	{ 	// in the case there are 2 children remove the smallest
		node->Data = RemoveSmallest(node->Right);
	}
	else {									// if there is 1 child node
		DeleteNode(node);
		return true;
	}
}

bool HW2BST::Remove(int dataValue)
{
	return Remove(m_root, dataValue);
}
