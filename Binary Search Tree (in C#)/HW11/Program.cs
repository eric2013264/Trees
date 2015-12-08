// Eric Chen 11381898

// Brief references for understanding principles: 
/// <references>    
/// http://stackoverflow.com/questions/13946740/is-there-any-method-like-foreach-for-ilist
/// https://miafish.wordpress.com/2015/01/28/leetcode-oj-c-binary-tree-inorder-traversal/
/// http://comsci.liu.edu/~murali/algo/Morris.htm
/// http://stackoverflow.com/questions/5502916/explain-morris-inorder-tree-traversal-without-using-stacks-or-recursion
/// http://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion-and-without-stack/
/// </references>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW10
{
    class Program
    {
        /// <summary>
        /// Main! I'm going to start using these fancy summary commment-things.
        /// </summary>
        static void Main(string[] args)
        {


            int i = 0;
            string input = "y",
                    prompt = "\nAgain (y/n)? ",
                    error_message = "Invalid input";

            while (input == "y" && input != "n")        // Generates 30 random numbers from 0-100 and adds to to a BST
            {
                Random rnd = new Random();
                BinarySearchTree bst = new BinarySearchTree();

                // 30 numbers from 0-100
                while (i < 30)
                {
                    int x = rnd.Next(0, 100);

                    if (!bst.search(bst.root, x))       // Check if its a duplicate number
                    {
                        bst.insert(bst.root, bst.addNode(x));
                        i++;
                    }
                }

                Console.Write("Traversal of tree using recursion: \n");
                bst.display(bst.root);                  // 1. Traversal using recursion

                Console.Write("\nTraversal of the tree using a stack but no recursion: \n");
                bst.In_Order_Traversal_2(bst.root);     // 2. Traversal using only stack and not recursion, O(n) time

                Console.Write("\nTraversal of the tree with NO stack and NO recursion: \n");
                bst.In_Order_Traversal_3(bst.root);     // 3. Traversal not using stack nor recursion!          

                /// <debug>
                /*
                Console.WriteLine("\n\n***** DEBUG *****");
                Console.WriteLine("TREE STATS FOR DEBUG: ");
                Console.WriteLine("  Number of nodes: {0}", BinarySearchTree.count);    // C# can't access static
                int level = bst.getLevel(bst.root); // Find max depth
                Console.WriteLine("  Number of levels: {0}", level);
                Console.Write("\nTREE CONTENTS: \n"); 
                bst.display(bst.root);
                Console.WriteLine("\n***** DEBUG *****\n");
                */
                /// </debug>
                /// 

                Console.WriteLine(prompt);      // Go again?
                input = Console.ReadLine();     // y/n

                bst = null;                     // Reset the bst
                i = 0;
            }
            while (input != "n")
            {
                Console.WriteLine(error_message);      // Invalid input
                Console.WriteLine(prompt);      // Go again?
                input = Console.ReadLine();     // y/n
            }

        }
    }
}
