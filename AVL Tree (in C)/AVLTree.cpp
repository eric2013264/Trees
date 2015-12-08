// Cpt S 223 Homework  - AVL Tree Implementation
// Eric Chen 11381898
// Implement the Add and Remove functions for the AVL tree
// Remember the rules for each node in an AVL tree:
// 1. Entire left subtree is less (same as BST)
// 2. Entire right subtree is greater (same as BST)
// 3. abs(DepthOfLeftSubtree - DepthOfRightSubtree) <= 1

#include "AVLTree.h"

using namespace std;

AVLTree::Node::Node(int dataValue, Node* parent)
{
	Data = dataValue;
	Left = Right = NULL;
	Parent = parent;
}

// Deletes a child of this node and sets the appropriate child pointer to 
// NULL. The child must be the immediate left or right child of this node.
bool AVLTree::Node::DeleteChild(Node* child)
{
	return ReplaceAndDeleteChild(child, NULL);
}

bool AVLTree::Node::ReplaceAndDeleteChild(Node* child, Node* newChild)
{
	if (child == Left)
	{
		if (Left) { delete Left; }
		Left = newChild;
	}
	else if (child == Right)
	{
		if (Right) { delete Right; }
		Right = newChild;
	}
	else
	{
		// It's not a child of this node
		return false;
	}
	return true;
}

AVLTree::AVLTree(void)
{
	m_root = NULL;
}

AVLTree::~AVLTree(void)
{
	DeleteTree(m_root);
	m_root = NULL;
}

// Should return false if the value already exists in the tree or if memory 
// for a new node could not be allocated.
bool AVLTree::Add(int dataValue)
{
	if (m_root)									// If we're not adding to an empty tree
	{
		Node* n = m_root;
		while (true)							// Check if memory could be allocated
		{
			if (dataValue == n->Data) 			// Check if value already in tree
			{
				return false;					// If yes, return false
			}

			else if (dataValue < n->Data)		// Less than, left
			{
				if (n->Left)
				{
					n = n->Left;
				}
				else
				{
					n->Left = new Node(dataValue, n);
					BalanceTree(n);				// We have to balance the tree first

					return true;
				}
			}
			else								// Greater than, right
			{
				if (n->Right)
				{
					n = n->Right;
				}
				else
				{
					n->Right = new Node(dataValue, n);
					BalanceTree(n);				// Balance the tree first

					return true;
				}
			}
		}
	}
	else										// Only node in the tree
	{
		m_root = new Node(dataValue, m_root);
		return true;
	}
}

// Adds all nodes, including null ones, that reside on the specified level. The 
// level number is 0-based. Since null nodes are included, this function always 
// adds exactly 2^level nodes to the queue. This is intended to be a helpful 
// utility function for the tree displaying function, which wants information 
// about everything on a level so it can compute spacing for a nice display.
void AVLTree::AddAllNodesOnLevel(int level, std::queue<Node*>& theQ)
{
	// Just return if level value is invalid
	if (level < 0)
	{
		return;
	}

	// Count the number of levels in the tree
	int lc = CountLevels();

	if (0 == level)
	{
		theQ.push(m_root);
	}
	else if (level >= lc)
	{
		// This means that we want nodes from a level that doesn't exist in the 
		// tree. So we just add 2^level null nodes to the queue.
		int count = 1 << level;
		while (count--)
		{
			theQ.push(NULL);
		}
	}
	else
	{
		// By no means is this the most efficient way to do this, but get the list of 
		// nodes for the previous level and just add their children.
		queue<Node*> prevLevel;
		AddAllNodesOnLevel(level - 1, prevLevel);

		while (!prevLevel.empty())
		{
			Node* parent = prevLevel.front();
			prevLevel.pop();

			// If the parent node is non-null, enqueue both children, otherwise enqueue 
			// 2 null children
			if (parent)
			{
				theQ.push(parent->Left);
				theQ.push(parent->Right);
			}
			else
			{
				theQ.push(NULL);
				theQ.push(NULL);
			}
		}
	}
}

// Utility function to compute the number of digits in the string representation of 
// the specified number in base-10.
int AVLTree::CountDigits(int decimalValue)
{
	if (0 == decimalValue)
	{
		return 1;
	}

	int count = (decimalValue < 0) ? 1 : 0;
	while (decimalValue)
	{
		count++;
		decimalValue /= 10;
	}
	return count;
}

// Returns the number of levels in the tree. A tree with no contents (null root) will have 
// zero levels. A tree with just the root and no children has 1 level. For all other cases 
// the number of levels must be counted. There is a linear-time algorithm for this.
int AVLTree::CountLevels()
{
	return CountLevels(m_root);
}

int AVLTree::CountLevels(Node* n)
{
	if (NULL == n)
	{
		return 0;
	}

	int leftLevels = CountLevels(n->Left);
	int rightLevels = CountLevels(n->Right);
	return 1 + ((leftLevels > rightLevels) ? leftLevels : rightLevels);
}

void AVLTree::DeleteTree(Node* root)
{
	if (root)
	{
		DeleteTree(root->Left);
		DeleteTree(root->Right);
		delete root;
	}
}

void AVLTree::DisplayContents(Node* node, ostream& outputStream)
{
	/*
	if (node)
	{
	DisplayContents(node->Left, outputStream);

	// Display the numerical value for this node
	outputStream << node->Data << " ";

	// Display the entire right subtree
	DisplayContents(node->Right, outputStream);
	}
	*/

	// Non-recusive version
	stack<Node*> nodes;
	while (true)
	{
		while (true)
		{
			if (!node) { break; }

			if (node->Left)
			{
				nodes.push(node);
				node = node->Left;
				continue;
			}

			outputStream << node->Data << " ";
			node = node->Right;
		}
		if (nodes.empty()) { break; }
		else
		{
			node = nodes.top();
			nodes.pop();
			outputStream << node->Data << " ";
			node = node->Right;
		}
	}
}

void AVLTree::DisplayContents(ostream& outputStream)
{
	outputStream << "BST Contents: ";
	DisplayContents(m_root, outputStream);
	outputStream << endl;
}

void AVLTree::DisplayLevels(std::ostream& outputStream)
{
	if (NULL == m_root)
	{
		outputStream << "(tree is empty)" << endl;
		return;
	}

	int lc = CountLevels();
	outputStream << "Tree with " << lc << " levels:" << endl;

	struct NodeLevel
	{
		Node* N;
		int Level;

		NodeLevel(Node* n, int lvl)
		{
			N = n;
			Level = lvl;
		}
	};

	bool worst = false;
	bool secondWorst = false;

	if (worst) // Worst display mode
	{
		queue<NodeLevel> nodes;
		nodes.push(NodeLevel(m_root, 0));
		int maxlevel = 0;
		while (!nodes.empty())
		{
			NodeLevel nl = nodes.front();
			nodes.pop();

			// See if we've hit a new level
			if (nl.Level > maxlevel)
			{
				outputStream << endl;
				maxlevel = nl.Level;
			}

			if (nl.N)
			{
				// Display the node's data and then enqueue children
				outputStream << nl.N->Data << " ";
				nodes.push(NodeLevel(nl.N->Left, nl.Level + 1));
				nodes.push(NodeLevel(nl.N->Right, nl.Level + 1));
			}
		}
	}
	else if (secondWorst) // Second worst display mode
	{
		for (int i = 0; i < CountLevels(); i++)
		{
			queue<Node*> theQ;
			AddAllNodesOnLevel(i, theQ);
			while (!theQ.empty())
			{
				Node* temp = theQ.front();
				theQ.pop();
				if (temp)
				{
					outputStream << temp->Data << " ";
				}
				else
				{
					outputStream << "{0} ";
				}
			}

			outputStream << endl;
		}
	}
	else // Smart display mode
	{
		// First find the maximum value in the tree and compute the number of digits
		// Note that we're assuming all positive numbers
		int max = GetMax();
		int charsPerNum = CountDigits(max);
		// We want at least one space after each number but also since rounding stuff 
		// gets weird we want a little extra padding
		charsPerNum += 2;
		// We display null values as "{0} " so we need at least 4 digits
		if (charsPerNum < 4) { charsPerNum = 4; }

		// Count the levels in the tree and then calculate how many characters will be 
		// needed to display the bottommost level.
		int lc = CountLevels();
		int charsOnLast = charsPerNum * (1 << (lc - 1));

		// Now loop through the rows and display them
		for (int i = 0; i < lc; i++)
		{
			queue<Node*> theQ;
			AddAllNodesOnLevel(i, theQ);

			float spacesBetweenNumCenters = (float)charsOnLast / (float)theQ.size();
			float numCenter = spacesBetweenNumCenters / 2.0f;
			int charAt = 0;

			while (!theQ.empty())
			{
				Node* temp = theQ.front();
				theQ.pop();
				int digitsForThisOne;
				if (temp)
				{
					digitsForThisOne = CountDigits(temp->Data);
				}
				else
				{
					digitsForThisOne = 3;
				}

				// Put right amount of spacing before this symbol
				int spaceCount = (int)(numCenter - (float)charAt - ((float)digitsForThisOne / 2.0f));
				if (spaceCount < 0)
				{
					bool breakhere = true;
				}
				if (spaceCount < 1) { spaceCount = 1; }
				DisplaySpaces(spaceCount, outputStream);

				// Display the number or symbol and update charAt and numCenter
				if (temp)
				{
					outputStream << temp->Data;
				}
				else
				{
					outputStream << "{0}";
				}
				charAt += spaceCount + digitsForThisOne;
				numCenter += spacesBetweenNumCenters;
			}

			outputStream << endl;
		}
	}
}

void AVLTree::DisplaySpaces(int spaceCount, ostream& outputStream)
{
	while (spaceCount--)
	{
		outputStream << " ";
	}
}

// Gets the maximum value in the tree. Returns 0 if the tree is empty.
int AVLTree::GetMax()
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

bool AVLTree::IsEmpty()
{
	return (NULL == m_root);
}

// Returns true if the value was found and removed, false if it was not found
bool AVLTree::Remove(int dataValue)
{
	if (!m_root)
	{
		return false;
	}

	Node* toDelete = m_root;

	while (true)								// Checks if memory could be allocated
	{
		if (toDelete->Data == dataValue)		// Match
		{
			break;
		}
		else if (dataValue > toDelete->Data)	// Greater than, right
		{
			if (toDelete->Right)
			{
				toDelete = toDelete->Right;
			}
			else
			{
				return false;
			}
		}
		else									// Less than, left
		{
			if (toDelete->Left)
			{
				toDelete = toDelete->Left;
			}
			else
			{
				return false;
			}
		}
	}

	Node* parent = toDelete->Parent;			// Parent of the node we want to delete

	// If the node to delete is a...

	if ((toDelete->Left == NULL) && (toDelete->Right == NULL)) // If the node to delete is a leaf
	{
		if (parent)
		{
			parent->DeleteChild(toDelete);
			BalanceTree(parent);
			return true;
		}

		delete m_root;
		m_root = NULL;
		return true;
	}

	else if (toDelete->Left == NULL)						// Root with only a right child
	{
		if (parent)
		{
			toDelete->Right->Parent = parent;
			parent->ReplaceAndDeleteChild(toDelete, toDelete->Right);
			BalanceTree(parent);
			return true;
		}

		m_root = m_root->Right;
		m_root->Parent = NULL;
		delete toDelete;
		return true;
	}

	else if (toDelete->Right == NULL)						// Root with only a left child
	{
		if (parent)
		{
			toDelete->Left->Parent = parent;
			parent->ReplaceAndDeleteChild(toDelete, toDelete->Left);
			BalanceTree(parent);
			return true;
		}

		m_root = m_root->Left;
		m_root->Parent = NULL;
		delete toDelete;
		return true;
	}

	else													// Non null on both sides
	{
		Node* min = toDelete->Right;
		while (min->Left)									// Find min value
		{
			min = min->Left;
		}
		int minValue = min->Data;
		Remove(minValue);
		toDelete->Data = minValue;
		BalanceTree(parent);
		return true;
	}

	return false;
}

void AVLTree::BalanceTree(Node* Parent)
{
	while (Parent)
	{
		if ((CountLevels(Parent->Left) - CountLevels(Parent->Right)) > 1)				// Left
		{
			if (CountLevels(Parent->Left->Left) >= CountLevels(Parent->Left->Right))	// Left
			{
				balanceLeftLeft(Parent);
			}
			else																		// Right
			{
				balanceRightRight(Parent->Left);
				balanceLeftLeft(Parent);
			}
		}
		else if ((CountLevels(Parent->Right) - CountLevels(Parent->Left)) > 1)			// Right
		{
			if (CountLevels(Parent->Right->Right) >= CountLevels(Parent->Right->Left))	// Right
			{
				balanceRightRight(Parent);
			}
			else																		// Left
			{
				balanceLeftLeft(Parent->Right);
				balanceRightRight(Parent);
			}
		}
		Parent = Parent->Parent;
	}
}

void AVLTree::balanceLeftLeft(Node* subRoot)
{
	Node* pivot = subRoot->Left;


	if (subRoot->Parent)							// SubRoot has a parent
	{
		if (subRoot->Parent->Right == subRoot)		// Right child
		{
			subRoot->Parent->Right = pivot;			// right->pivot
		}
		else										// Left child
		{
			subRoot->Parent->Left = pivot;			// left->pivot
		}
	}
	else											// SubRoot doesn't have a parent
	{
		m_root = pivot;								// root = pivot
	}

	pivot->Parent = subRoot->Parent;				// Set pivot's parent = subRoot's parent
	subRoot->Parent = pivot;						// Set subRoot's parent = pivot

	subRoot->Left = pivot->Right;					// Set subRoot's left child to be pivot's right child
	if (subRoot->Left)								// Check if left child exists
	{
		subRoot->Left->Parent = subRoot;			// Set parent as subRoot
	}

	pivot->Right = subRoot;							// SubRoot fills the newly emptied pivot->left

	return;
}
void AVLTree::balanceRightRight(Node* subRoot)
{
	Node* pivot = subRoot->Right;

	if (subRoot->Parent)							// If subRoot has a parent
	{
		if (subRoot->Parent->Right == subRoot)		// And is the right child
		{
			subRoot->Parent->Right = pivot;			// Set pivot as right child
		}
		else										// And is the left child
		{
			subRoot->Parent->Left = pivot;			// Set pivot as left child
		}
	}
	else											// SubRoot has no parent
	{
		m_root = pivot;								// Set pivot as the new root of the tree.
	}

	pivot->Parent = subRoot->Parent;				// Set pivot's parent to subRoot's parent
	subRoot->Parent = pivot;						// Set subRoot's parent = pivot

	subRoot->Right = pivot->Left;					// Set subRoots's right child to be pivot's left child
	if (subRoot->Right)								// Check if right child exists
	{
		subRoot->Right->Parent = subRoot;			// Set parent as subRoot
	}

	pivot->Left = subRoot;							// SubRoot fills the newly emptied pivot->left

	return;
}