#include "red_black_tree.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

/*
 * Create a new red black tree having a root value of value
 */
red_black_tree_t*
init_red_black_tree(rb_node_value_cmp node_comparer, rb_node_value_destructor dest, rb_node_copy_value copy_fn)
{
	red_black_tree_t* tree = (red_black_tree_t*)malloc(sizeof(red_black_tree_t));

	if (!tree)
		return NULL;

	tree->head = NULL;
	tree->node_comparer = node_comparer;
	tree->value_destructor = dest;
	tree->value_copy = copy_fn;

	return tree;
}

/*
 * Recursively delete the node and it's children
 */
void
rb_delete_node(red_black_tree_t* t, rb_node_t* node)
{
	if (node)
	{
		if (node->left_node)
			rb_delete_node(t, node->left_node);

		if (node->right_node)
			rb_delete_node(t, node->right_node);

		t->value_destructor(node->value);

		free(node);
		node = NULL;
	}
}

/*
 * Delete the tree and all it's children
 */
void
rb_delete_tree(red_black_tree_t* t)
{
	if (!t)
		return;

	if (t->head)
	{
		rb_delete_node(t, t->head);
	}

	free(t);
	t = NULL;
}

/*
 * Get the color of a node/if node == null then the color is black
 */
enum node_color
rb_get_node_color(rb_node_t* node)
{
	if (!node) return RB_BLACK;

	return node->color;
}

void 
left_rotate_rb_node(red_black_tree_t* t, rb_node_t* node)
{
	if (node && node->right_node != NULL)
	{
		rb_node_t* y = node->right_node;

		node->right_node = y->left_node;

		if (y->left_node)
			y->left_node->parent_node = node;

		y->parent_node = node->parent_node;

		if (node->parent_node == NULL)
		{
			t->head = y;
		} else if (node == node->parent_node->left_node)
		{
			node->parent_node->left_node = y;
		} else 
			node->parent_node->right_node = y;

		y->left_node =  node;
		node->parent_node = y;
	}
}

void 
right_rotate_rb_node(red_black_tree_t* t, rb_node_t* node)
{
	if (node && node->left_node != NULL)
	{
		rb_node_t* y = node->left_node;

		node->left_node = y->right_node;

		if (y->right_node)
			y->right_node->parent_node = node;

		y->parent_node = node->parent_node;

		if (node->parent_node == NULL)
		{
			t->head = y;
		} else if (node == node->parent_node->right_node)
		{
			node->parent_node->right_node = y;
		} else 
			node->parent_node->left_node = y;

		y->right_node =  node;
		node->parent_node = y;
	}
}

void
rb_insert_fixup(red_black_tree_t* t, rb_node_t* z)
{
	rb_node_t* y;

	while (rb_get_node_color(z->parent_node) == RB_RED)
	{
		if (z->parent_node == z->parent_node->parent_node->left_node)
		{
			y = z->parent_node->parent_node->right_node;

			if (rb_get_node_color(y) == RB_RED)
			{
				z->parent_node->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent_node->parent_node->color = RB_RED;
				z = z->parent_node->parent_node;
			} else {
				if (z == z->parent_node->right_node)
				{
					z = z->parent_node;
					left_rotate_rb_node(t, z);
				}

				z->parent_node->color = RB_BLACK;
				z->parent_node->parent_node->color = RB_RED;
				right_rotate_rb_node(t, z->parent_node->parent_node);
			}
		} else {
			y = z->parent_node->parent_node->left_node;

			if (rb_get_node_color(y) == RB_RED)
			{
				z->parent_node->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent_node->parent_node->color = RB_RED;
				z = z->parent_node->parent_node;
			} else {
				if (z == z->parent_node->left_node)
				{
					z = z->parent_node;
					right_rotate_rb_node(t, z);
				}

				z->parent_node->color = RB_BLACK;
				z->parent_node->parent_node->color = RB_RED;
				left_rotate_rb_node(t, z->parent_node->parent_node);
			}
		}
	}

	t->head->color = RB_BLACK;
}

void
rb_insert(red_black_tree_t* t, rb_node_t* z)
{
	rb_node_t* y = NULL;
	rb_node_t* x = t->head;

	while (x != NULL)
	{
		y = x;

		if (t->node_comparer(z->value, x->value) < 0)
			x = x->left_node;
		else
			x = x->right_node;
	}

	z->parent_node = y;

	if (y == NULL)
		t->head = z;
	else 
	{
		if (t->node_comparer(z->value, y->value) < 0)
			y->left_node = z;
		else
			y->right_node = z;
	}

	z->left_node = NULL;
	z->right_node = NULL;
	z->color = RB_RED;

	rb_insert_fixup(t, z);
}

void
rb_insert_value(red_black_tree_t* t, void* value)
{
	rb_node_t* node = NULL;

	if (!t)
		return;

	node = malloc(sizeof(rb_node_t));

	if (node)
	{
		node->parent_node = NULL;
		node->left_node = NULL;
		node->right_node = NULL;
		node->value = value;
	
		rb_insert(t, node);
	}
}

void*
rb_min_value(red_black_tree_t* t)
{
	rb_node_t* x = NULL;

	if (t)
	{
		x = t->head;

		if (x)
		{
			while (x->left_node != NULL)
			{
				x = x->left_node;
			}

			return x->value;
		}
	}

	return 0;
}

void*
rb_max_value(red_black_tree_t* t)
{
	rb_node_t* x = NULL;

	if (t)
	{
		x = t->head;

		if (x)
		{
			while (x->right_node != NULL)
			{
				x = x->right_node;
			}

			return x->value;
		}
	}

	return 0;
}


rb_node_t*
rb_tree_minimum(rb_node_t* x)
{
	if (x)
	{
		while (x->left_node != NULL)
		{
			x = x->left_node;
		}

		return x;
	}

	return NULL;
}

rb_node_t*
rb_tree_maximum(rb_node_t* x)
{
	if (x)
	{
		while (x->right_node != NULL)
		{
			x = x->right_node;
		}

		return x;
	}

	return NULL;
}

rb_node_t*
rb_tree_succ(rb_node_t* x)
{
	rb_node_t* y = NULL;

	if (x)
	{
		if (x->right_node != NULL)
			return rb_tree_minimum(x);

		y = x->parent_node;

		while (y != NULL && x == y->right_node)
		{
			x = y;
			y = y->parent_node;
		}

		return y;
	}

	return NULL;
}

void
rb_delete_node_from_tree_fixup(red_black_tree_t* t, rb_node_t* x, rb_node_t* x_parent)
{
	rb_node_t* w = NULL;

	while (x != t->head && rb_get_node_color(x) == RB_BLACK)
	{
		if (x == x_parent->left_node)
		{
			w = x_parent->right_node;

			if (rb_get_node_color(w) == RB_RED)
			{
				w->color = RB_BLACK;
				x_parent->color = RB_RED;

				left_rotate_rb_node(t, x_parent);
				w = x_parent->right_node;
			}

			if (w && rb_get_node_color(w->left_node) == RB_BLACK && rb_get_node_color(w->right_node) == RB_BLACK)
			{
				w->color = RB_RED;
				x = x_parent;
			} else {
				if (rb_get_node_color(w->right_node) == RB_BLACK) {
					w->left_node->color = RB_BLACK;
					w->color = RB_RED;

					right_rotate_rb_node(t, w);
					w = x_parent->right_node;
				}

				w->color = rb_get_node_color(x_parent);
				w->right_node->color  = RB_BLACK;

				x_parent->color = RB_BLACK;
				left_rotate_rb_node(t, x_parent);

				x = t->head;
			}
		} else 
		{
			w = x_parent->left_node;

			if (rb_get_node_color(w) == RB_RED)
			{
				w->color = RB_BLACK;
				x_parent->color = RB_RED;

				right_rotate_rb_node(t, x_parent);
				w = x_parent->left_node;
			}

			if (w && rb_get_node_color(w->right_node) == RB_BLACK && rb_get_node_color(w->left_node) == RB_BLACK)
			{
				w->color = RB_RED;
				x = x_parent;
			} else {

				if (w)
				{
					if (rb_get_node_color(w->left_node) == RB_BLACK) {
						w->right_node->color = RB_BLACK;
						w->color = RB_RED;

						left_rotate_rb_node(t, w);
						w = x_parent->left_node;
					}

					w->color = rb_get_node_color(x_parent);
					w->left_node->color  = RB_BLACK;
				}

				x_parent->color = RB_BLACK;

				right_rotate_rb_node(t, x_parent);

				x = t->head;
			}
		}
	}

	if (x)
		x->color = RB_BLACK;
}

/*
 * Delete a node from the tree.
 */
rb_node_t*
rb_delete_node_from_tree(red_black_tree_t* t, rb_node_t* z)
{
	rb_node_t* y = NULL;
	rb_node_t* x = NULL;
	rb_node_t* x_parent = NULL;

	if (!z)
		return NULL;

	if (z->left_node == NULL || z->right_node == NULL)
		y = z;
	else
		y = rb_tree_succ(z);

	if (!y)
		return NULL;

	if (y->left_node != NULL)
		x = y->left_node;
	else 
		x = y->right_node;

	if (x)
		x->parent_node = y->parent_node;
	
	x_parent = y->parent_node;

	if (y->parent_node == NULL)
	{
		t->head = x;
	} else 
	{
		if (y == y->parent_node->left_node)
			y->parent_node->left_node = x;
		else
			y->parent_node->right_node = x;
	}

	if (y != z)
	{
		t->value_copy(y->value, z->value);
	}

	if (x)
	if (rb_get_node_color(y) == RB_BLACK)
		rb_delete_node_from_tree_fixup(t, x, x_parent); 

	return y;
}

rb_node_t*
rb_delete_max_element(red_black_tree_t* t)
{
	rb_node_t* max_node = rb_tree_maximum(t->head);

	if (max_node)
		return rb_delete_node_from_tree(t, max_node);

	return NULL;
}

rb_node_t*
rb_delete_min_element(red_black_tree_t* t)
{
	rb_node_t* min_node = rb_tree_minimum(t->head);

	if (min_node)
		return rb_delete_node_from_tree(t, min_node);

	return NULL;
}

