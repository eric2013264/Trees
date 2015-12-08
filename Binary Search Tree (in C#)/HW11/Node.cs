// Eric Chen 11381898

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW10
{
    /// <summary>
    /// Public int Node Class from HW1. Left. Right. Data.
    /// </summary>
    class Node
    {
        public int data;
        public Node left;
        public Node right;

        public Node(int value)
        {
            data = value;
            left = null;
            right = null;
        }
    }
}
