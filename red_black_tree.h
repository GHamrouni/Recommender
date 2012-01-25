/*-
* Copyright (c) 2012 Hamrouni Ghassen.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/************************************************************************/
/*	Red-black tree implementation			                */
/*	As described in Introduction to Algorithms. Second Edition	*/
/************************************************************************/

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

enum node_color	/* Defines red black tree node color */
{
	RB_RED,
	RB_BLACK
};


/*
 * Define node-value destructor/it will be called when releasing a node-value.
 */
typedef void (*rb_node_value_destructor)(void *);

/*
 * Define a weak strict order between nodes.
 */
typedef int (*rb_node_value_cmp)(const void *, const void *);

/*
 * Define a copy operator dest = src.
 */
typedef void (*rb_node_copy_value)(const void* src, void* dest);


/* A red black tree node
 * It contains an additional parameter
 * the color used for balancing the tree.
 */
typedef struct rb_node
{
	void*			value;

	enum node_color		color;

	struct rb_node*		parent_node;
	struct rb_node*		left_node;
	struct rb_node*		right_node;

} rb_node_t;


/*
 * A red black tree is a balanced binary tree.
 */
typedef struct red_black_tree
{
	rb_node_t*			head;

	rb_node_value_cmp		node_comparer;

	rb_node_copy_value		value_copy;

	rb_node_value_destructor	value_destructor;

} red_black_tree_t;


/*
 * Create a new red black tree having a root value of value
 */
red_black_tree_t*
init_red_black_tree(rb_node_value_cmp node_comparer, 
		rb_node_value_destructor dest, rb_node_copy_value copy_fn);

/*
 * Delete the entire tree and all it's nodes
 */
void
rb_delete_tree(red_black_tree_t* t);

/*
 * Delete a node from the tree.
 */
rb_node_t*
rb_delete_node_from_tree(red_black_tree_t* t, rb_node_t* node);

/*
 * Get the color of a node/if node == null then the color is black
 */
enum node_color
rb_get_node_color(rb_node_t* node);

void
rb_insert(red_black_tree_t* t, rb_node_t* z);

/*
 * Insert a value to tree
 */
void
rb_insert_value(red_black_tree_t* t, void* value);

/*
 * Get the minimal value in the tree
 */
void*
rb_min_value(red_black_tree_t* t);

/*
 * Get the maximal value in the tree
 */
void*
rb_max_value(red_black_tree_t* t);

/*
 * Delete the node having the maximum value
 */
rb_node_t*
rb_delete_max_element(red_black_tree_t* t);

/*
 * Delete the node having the minimum value
 */
rb_node_t*
rb_delete_min_element(red_black_tree_t* t);

#endif /* RED_BLACK_TREE_H */

