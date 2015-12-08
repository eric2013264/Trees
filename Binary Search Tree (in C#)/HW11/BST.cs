// Eric Chen 11381898

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW10
{
    /// <summary>
    /// Binary Search Tree class. Main functionality (addNode, insert, etc) recycled from my very own hw1!
    /// </summary>
    class BinarySearchTree
    {
        public Node root;
        public static int count;
        public BinarySearchTree() { root = null; }


        /// <summary>
        /// Creates and returns a BST node
        /// </summary>
        public Node addNode(int data)
        {
            Node temp = new Node(data);

            if (root == null)
            {
                root = temp;
            }
            count++;

            return temp;
        }

        /// <summary>
        /// Inserts the new node in the right place
        /// </summary>
        public void insert(Node root, Node newNode)
        {
            while (root != null)
            {
                if (newNode.data > root.data)
                {
                    if (root.right == null)
                    {
                        root.right = newNode;
                        break;
                    }
                    root = root.right;
                }
                else
                {
                    if (root.left == null)
                    {
                        root.left = newNode;
                        break;
                    }
                    root = root.left;
                }
            }
        }

        /// <summary>
        /// Clear it all, clear it all!
        /// </summary>
        public void clear(Node root) //get root
        {
            if (root == null)
            {
                return;
            }
            if (root.left != null)
            {
                clear(root.left);
            }
            if (root.right != null)
            {
                clear(root.right);
            }
            root = null;

            return;
        }

        /// <summary>
        /// Helper function (checks for duplicate node before adding) that recursively searches the BST given an int value.
        /// Compares values to root first to save time.
        /// </summary>
        public bool search(Node root, int x)
        {
            if (root == null)           // Case 1: Tree is empty
            {
                return false;
            }
            else if (root.data == x)    // Case 2: Equal to root, duplicate found.
            {
                return true;
            }
            else if (root.data > x)
            {   // Case 3: Less than root, recurse left
                return search(root.left, x);
            }
            else                        // Case 4: Greater than root, recurse right
                return search(root.right, x);

        }

        /// <summary>
        /// 1. Function that recursively displays the tree inorder from least to greatest.
        /// </summary>
        public void display(Node root)
        {
            if (root != null)
            {
                display(root.left);
                Console.Write(root.data + " ");
                display(root.right);
            }
        }

        /// <summary>
        /// Function that finds the max depth of the BST recursively
        /// </summary>
        public int getLevel(Node root)
        {
            if (root == null)
            {
                return 0;
            }
            else
            {
                int leftLevel = getLevel(root.left);
                int rightLevel = getLevel(root.right);

                if (rightLevel > leftLevel)
                {
                    return (rightLevel + 1);
                }
                else
                    return (leftLevel + 1);
            }
        }

        /// <summary>
        /// 2. Non-recursive function that uses a stack to traverse the BST in order. 
        /// 
        /// Algorithm/function given by Evan Olds in lecture 11/13:
        /// 
        /// public void In_Order_Traversal_2(Node root)
        /// {
        ///     while (true)
        ///     {
        ///         stack<node> nodes = new Stack<node>();
        ///
        ///         while(n != null)
        ///         {
        ///             nodes.Push(n);
        ///             n=n.Left;
        ///         }
        ///
        ///         if (nodes.Count == 0){break;}
        ///
        ///         n = nodes.Pop()
        ///         console.write(n.Data.ToString() + " ");
        ///         n=n.right)
        ///     }
        ///     console.writeline();
        /// }

        /// </summary>
        public void In_Order_Traversal_2(Node root)
        {
            Stack<Node> s = new Stack<Node>();
            Node current_node = root;   // Isn't necessary, we could just parameter root. Since it's only declared once, why not for
            bool exit = false;          // accurate variable names' sake.

            if (root == null)                       // Check for empty tree
            {
                return;
            }

            while (s != null && !exit)
            {
                if (current_node != null)
                {
                    s.Push(current_node);
                    current_node = current_node.left;   // Lowest lefthand node
                }
                else
                {
                    if (s.Count == 0)
                    {
                        exit = true;
                    }
                    else
                    {
                        current_node = s.Peek();  // Set the current node to the top element of stack w/o adjusting the stack. eg Pop() won't work
                        s.Pop();
                        Console.Write(current_node.data);
                        Console.Write(" ");

                        current_node = current_node.right;
                    }
                }
            }

        }

        /// <summary>
        /// 3. Function that doesn't use recursion nor stack to do an inorder traversal of the BST.
        /// Apparently this algorithm is called the "Morris traversal" method. The resources I used to understand and implement it are cited in my references.
        /// Although modifying the tree is necessary, it's reverted after traversal is done, as per the assignemtn requirements.
        /// This algorithm, like Evan hinted, uses the null leaf nodes to determine when to stop and when to begin traversing left or right child nodes.
        /// </summary>
        public void In_Order_Traversal_3(Node root)
        {
            if (root != null)
            {
                Node current_node = root, prev_node = null;

                while (current_node != null)
                {
                    if (current_node.left == null)  // Found furthest left, print
                    {
                        Console.Write(current_node.data);
                        Console.Write(" ");

                        current_node = current_node.right;      // Next!
                    }
                    else
                    {
                        prev_node = current_node.left;

                        while (prev_node.right != current_node && prev_node.right != null)
                        {
                            prev_node = prev_node.right;        // Find the previous node
                        }

                        if (prev_node.right == null)
                        {
                            prev_node.right = current_node;     // Because prev_node.right can't be equal to current_node
                            current_node = current_node.left;
                        }
                        else // Revert the bst
                        {
                            Console.Write(current_node.data);
                            Console.Write(" ");

                            current_node = current_node.right;  // Next!
                            prev_node.right = null;
                        }
                    }
                }
            }
            else    // Empty tree
            {
                return;
            }
        }

    }
}
