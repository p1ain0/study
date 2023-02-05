/*
红黑树的性质：
1.每个结点是红的或者黑的
2.根节点是黑的
3.每个叶子结点是黑的
4.如果一个结点是红的，则它的两个儿子都是黑的
5.对每个结点，从该结点到其子孙结点的所有路径上的包含相同数目的黑结点
*/


#include <stdio.h>
#include <stdlib.h>
#define RED 1
#define BLACK 2

typedef int KEY_TYPE;

typedef struct _rbtree_node_t {
	KEY_TYPE key;
	int color;
	struct _rbtree_node_t* left;
	struct _rbtree_node_t* right;
	struct _rbtree_node_t* parent;
}rbtree_node_t;

typedef struct _rbtree_t {
	rbtree_node_t* root;
	rbtree_node_t* nil;
}rbtree_t;



void rbtree_left_rotate(rbtree_t* T, rbtree_node_t* x)
{
	rbtree_node_t* y = x->right;
	x->right = y->left;
	if (y->left != T->nil)
		y->left->parent = x;
	y->left = x;
	if (x->parent == T->nil)
		T->root = y;
	else if (x->parent->left == x)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->parent = x->parent;
	x->parent = y;
}

void rbtree_right_rotate(rbtree_t* T, rbtree_node_t* x)
{
	rbtree_node_t* y = x->left;
	x->left = y->right;
	if (y->right != T->nil)
		y->right->parent = x;
	y->right = x;

	if (x->parent == T->nil)
		T->root = y;
	else if (x->parent->left == x)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->parent = x->parent;
	x->parent = y;
}

/*
添加一个结点，插入这个节点的颜色，是红色更容易满足红黑树的性质。
哪些条件下需要进行调整
当前节点是红色，同时父节点也是红色，祖父节点一定是黑的：
1.叔父节点为红色
2.叔父节点为黑色，且当前节点是右孩子
3.叔父节点为黑色，且当前节点是左孩子
*/

void rbtree_insert_fixup(rbtree_t* T, rbtree_node_t* z)
{
	while (z->parent != T->nil && z->parent->color == RED)
	{
		if (z->parent->parent->left == z->parent)
		{
			if (z->parent->parent->right != T->nil && z->parent->parent->right->color == RED)
			{
				z->parent->parent->color = RED;
				z->parent->parent->right->color = BLACK;
				z->parent->color = BLACK;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					rbtree_left_rotate(T, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_right_rotate(T, z->parent->parent);
			}
		}
		else
		{
			if (z->parent->parent->left != T->nil && z->parent->parent->left->color == RED)
			{
				z->parent->parent->color = RED;
				z->parent->parent->left->color = BLACK;
				z->parent->color = BLACK;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					rbtree_right_rotate(T, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_left_rotate(T, z->parent->parent);
			}
		}
	}
	T->root->color = BLACK;
}


void rbtree_insert(rbtree_t* T, rbtree_node_t* z)
{
	rbtree_node_t* x = T->root;
	rbtree_node_t* y = x;
	while (x != T->nil)
	{
		y = x;
		if (x->key < z->key)
			x = x->right;
		else if (x->key > z->key)
			x = x->left;
		else
			return;
	}
	z->parent = y;
	if (y == T->nil)
		T->root = z;
	else if (y->key < z->key)
		y->right = z;
	else
		y->left = z;
	z->left = T->nil;
	z->right = T->nil;
	z->color = RED;
	rbtree_insert_fixup(T, z);
}

void rbtree_travel(rbtree_node_t* x)
{
	if (x != NULL)
	{
		rbtree_travel(x->left);
		printf("%d\n", x->key);
		rbtree_travel(x->right);
	}
}

int main()
{
	rbtree_t T;
	T.root = NULL;
	T.nil = NULL;
	int a[10] = { 100, 103, 1, 10, 1000, 5, 6, 7, 8, 0 };
	for (int i = 0; i < 10; i++)
	{
		rbtree_node_t* node = calloc(1, sizeof(rbtree_node_t));
		node->key = a[i];
		rbtree_insert(&T, node);
	}
	rbtree_travel(T.root);

	return 0;
}