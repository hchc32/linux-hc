#include <iostream>
using namespace std;

typedef struct Node
{
    char data;
    struct Node *left;
    struct Node *right;
}TreeNode;



void Mirror(TreeNode *pRoot) 
{
        if(pRoot == nullptr)
            return;
        queue<TreeNode*> q;
        q.push(pRoot);

        while(!q.empty())
        {
            TreeNode* node = q.front();
            q.pop();

            if(node->left != nullptr || node->right != nullptr)
            {
                TreeNode* temp = node->left;
                node->left = node->right;
                node->right = temp;
            }

            if(node->left != nullptr) q.push(node->left);
            if(node->right != nullptr) q.push(node->right);
        }

}

int main()
{
    TreeNode* root;
    Mirror(root);
    return 0;
}
