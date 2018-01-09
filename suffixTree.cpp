#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <iostream>
using namespace std;
string text = "";
struct node{
	node *(child[26]);
	node *suffixlink;
	node *suffixlinkparent;
	node *parent;
	int start;
	int *end;
	int child_ct;
	int ct;
	int linkind[2];
};
node *root = new node;
void Linking(node *ptr1, node *ptr2);
void init_root(){
	int i;
	for(i=0; i<10; i++)
		root->child[i] = NULL;
	root->child_ct = 0;
	root->suffixlink = root;
	root->suffixlinkparent = root;
}

void new_node(node *x, int start, int *end){
	node *ptr = new node;
	ptr->child_ct = 0;
	x->child[x->child_ct] = ptr; 
	x->child_ct = x->child_ct + 1;
	ptr->suffixlink = root;
	ptr->suffixlinkparent = root;
	ptr->start = start;
	ptr->end = end;
	ptr->parent = x;
}

node* search_node(char c, node *x, string text){
	int i;
	for(i=0; i<x->child_ct; i++){
		if(text[x->child[i]->start] == c)
			return x->child[i];
	}
	return NULL;
}

node *activeNode = root;
node *activeEdge = root;
int activeLength = 0;
int rem = 0;
int leafnode;

void modify(){
	if(activeEdge == root)
		return;
	int len = *(activeEdge->end) - activeEdge->start;
	if(activeLength > len){
		activeNode = activeEdge;
		activeEdge = search_node(text[activeEdge->start + len], activeNode, text);
		activeLength -= len;
		modify();
	}
	else if(activeLength == len){
		activeNode = activeEdge;
		activeLength -= len;
	}
}

void end_val(node *x, int end){
	int *f = new int;
	*f = end;
	x->end = f;
}
void print_node(node *x){
	int i;
	if(x == root)
		cout << "-1\n";
	else{
		for(i=x->start; i<*(x->end); i++)
			cout << text[i];
		cout << endl;
	}
}

void print(node *x){
	int i;
	print_node(x);
	for(i=0; i<x->child_ct; i++)
		print(x->child[i]);
}

void extendTree(int pos){
	node *ptr1, *ptr2;
	rem++;
	int i;
	node *searchNode;
	leafnode = pos + 1;
	while(rem != 0){
		modify();
		if(activeEdge == activeNode){ 
			searchNode = search_node(text[pos], activeNode, text);
			if(searchNode != NULL){
				//cout << "E1\n";
				activeEdge = searchNode;
				activeLength += 1;
				modify();
				break;
			}
			else{
				//cout << "E2\n";
				rem--;
				new_node(activeNode, pos, &leafnode);
				activeNode = activeNode->suffixlink;
				if(activeNode == root){
					activeLength = 0;
					activeEdge = root;
				}
				else{
					activeEdge = activeNode->child[activeEdge->ct];
				}
			}
		}
		else{
			if(text[activeEdge->start + activeLength] == text[pos]){
				//cout << "E3\n";
				activeLength += 1;
				modify();
				break;
			}
			else{
				//cout << "E4\n";
				rem--;
				modify();
				ptr1 = activeEdge;
				node *ptr = new node;
				if(ptr1->child_ct >= 2){
					for(i=0; i<ptr1->child_ct; i++)
						ptr->child[i] = ptr1->child[i];
					ptr->child_ct = ptr1->child_ct;
					ptr1->child_ct = 0;
					ptr1->child[ptr1->child_ct++] = ptr;
					ptr->parent = ptr1;
					ptr->start = activeEdge->start + activeLength;
					ptr->end = activeEdge->end;
					if(ptr1->suffixlink != root){
						Linking(ptr,ptr1->suffixlink);
						ptr1->suffixlink = root;
					}
					if(ptr1->suffixlinkparent != root){
						Linking(ptr1->suffixlinkparent->child[0], ptr);
					}

				}
				else
					new_node(activeEdge, activeEdge->start + activeLength, activeEdge->end);
				new_node(activeEdge, pos, &leafnode);
				end_val(activeEdge, activeEdge->start + activeLength);
				if(activeNode == root){
					activeEdge = search_node(text[activeEdge->start + 1], activeNode->suffixlink, text);
					activeLength--;
					activeNode = activeNode->suffixlink;
				}
				else if(activeNode->suffixlink == root){
					if(text[activeNode->start] == text[activeEdge->start]){
						activeLength--;
						activeEdge = search_node(text[activeEdge->start + 1], activeNode, text);
					}
					else{
						activeNode = activeNode->parent;
						activeEdge = search_node(text[pos - activeLength], activeNode, text);
					}
				}
				else{
					activeEdge = search_node(text[activeEdge->start], activeNode->suffixlink, text);
					activeNode = activeNode->suffixlink;
				}
				modify();
				if(activeLength == 0)
					activeEdge = activeNode;
				ptr2 = activeEdge;
				Linking(ptr1, ptr2);
			}
		}
	}
	
}
void Linking(node *ptr1, node *ptr2){
	if(ptr2!=root){
		ptr1->suffixlink = ptr2;
		ptr2->suffixlinkparent = ptr1;
	}
}

void print_links(node *x){
	int i;
	if(x->suffixlink != root){
		print_node(x);
		print_node(x->suffixlink);
		cout << endl;
	}
	for(i=0; i<x->child_ct; i++)
		print_links(x->child[i]);
}

int main(){
	cin >> text;
	unsigned int i;
	init_root();
	for(i=0; i<text.length(); i++)
		extendTree(i);
	print(root);
	return 0;
}
