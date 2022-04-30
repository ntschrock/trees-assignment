#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------STRUCTURE DECLARATIONS----------------------------------------------*/
//item node structure
struct item_node_struct
{
	char name[32];
	int count;
	struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

//tree name structure
struct tree_name_node_struct
{
	char treeName[32];
	struct tree_name_node_struct *left, *right;
	item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

/*----------------------------------------------SEARCH FUNCTIONS----------------------------------------------*/
//required function that finds a sub tree in the top half
tree_name_node *search_for_name_node(tree_name_node *namenode, char *tree_name) 
{
  	tree_name_node *temp;
	
	if(namenode == NULL)
	{
		return NULL;
	}
  	if(strcmp(namenode-> treeName, tree_name) == 0)
	{
		return namenode;
	}

	temp = search_for_name_node(namenode-> left, tree_name);
  	if(temp == NULL)
	{
		return search_for_name_node(namenode-> right, tree_name);
	}
  	return temp;
}

//required function that finds an item in the bottom half
item_node *search_in_name_node(item_node *itemnode, char *item_name)
{
  	item_node *temp;
	
	if(itemnode == NULL)
	{
    	return NULL;
	}
  	if(strcmp(itemnode-> name, item_name) == 0)
	{
    	return itemnode;
	}

	temp = search_in_name_node(itemnode-> left, item_name);
  	if(temp == NULL)
	{
		return search_in_name_node(itemnode-> right, item_name);
	}
  	return temp;
}

//uses both functions to find if an item exists, if it does not prints that it doesnt
item_node *search_all(FILE *ofp, tree_name_node *namenode, char *tree_name, char *item_name) 
{
 	tree_name_node *tree;
	item_node *item;
	
	tree = search_for_name_node(namenode, tree_name);
	if(tree == NULL) 
	{
    	fprintf(ofp, "%s does not exist\n", tree_name);
    	return NULL;
  	}

	item = search_in_name_node(tree-> theTree, item_name);
  	if(item == NULL) 
	{
    	fprintf(ofp, "%s not found in %s\n", item_name, tree_name);
		return NULL;
  	}
  	return item;
}
/*----------------------------------------------FUNCTIONS FOR ITEM NODE----------------------------------------------*/
//function to create a new item node
item_node *create_item_node()
{
	item_node *itemnode = malloc(sizeof(item_node));

	strcpy(itemnode-> name, "");
	itemnode-> count = 0;
	itemnode-> left = NULL;
	itemnode-> right = NULL;

	return itemnode;
}

//prints the names under the tree header, will work its way through the item node
void print_item_names(FILE *ofp, item_node *itemnode) 
{
  	if(itemnode != NULL)
	{ //needs to be in this order or it will make the output in a different order
    	print_item_names(ofp, itemnode-> left);
  		fprintf(ofp, "%s ", itemnode-> name);
  		print_item_names(ofp, itemnode-> right);
	}
	else
	{
		return;
	}
}

//tree insertion, will return the root of the tree
item_node *binary_tree_insertion_item(item_node *parent, item_node *root, item_node *itemnode) 
{
  	if(root == NULL)
	{
		return itemnode;
	}
    
  	if(strcmp(itemnode-> name, parent-> name) > 0) 
	{
    	if(parent-> right == NULL)
		{
			parent-> right = itemnode; 
		}
    	else
		{
			return binary_tree_insertion_item(parent-> right, root, itemnode);
		}
  	}
  	else 
	{
    	if(parent-> left == NULL)
		{
			parent-> left = itemnode;
		}
    	else
		{
			return binary_tree_insertion_item(parent-> left, root, itemnode);
		}
  	}
  	return root;
}

//uses pervious function to insert
void insert(tree_name_node *root, char *parent_name, item_node *itemnode) 
{
  	tree_name_node *parent;

	parent = search_for_name_node(root, parent_name);
  	parent-> theTree = binary_tree_insertion_item(parent-> theTree, parent-> theTree, itemnode);
}

//finds minimum item node
item_node *find_minimum_item_node(item_node *itemnode) 
{
  	if(itemnode == NULL)
	{
		return NULL;
	}
    
  	if(itemnode-> left == NULL)
	{
		return itemnode;
	}
	else
	{
		return find_minimum_item_node(itemnode-> left);
	}
}

//gets rid of an item node
item_node *delete_item_node(item_node *itemnode, char *item_name) 
{
	item_node *temp;
    item_node *minimum;
  	
	if(itemnode == NULL)
	{
		return NULL;
	}
  	else if(strcmp(item_name, itemnode-> name) > 0)
	{
		itemnode-> right = delete_item_node(itemnode-> right, item_name);
	}
	else if(strcmp(item_name, itemnode-> name) < 0)
	{
		itemnode-> left = delete_item_node(itemnode-> left, item_name);
	}
  	else 
	{ 
    	if(itemnode-> left == NULL) 
		{
      		temp = itemnode-> right;
      		return temp;
    	}
    	else if(itemnode-> right == NULL) 
		{
      		temp = itemnode-> left;
      		return temp;
    	}
    	else 
		{
			minimum = find_minimum_item_node(itemnode-> right);
      		itemnode-> count = minimum-> count;
      		
			strcpy(itemnode-> name, minimum-> name);
      		itemnode-> right = delete_item_node(itemnode-> right, minimum-> name);
    	}
  	}
  	return itemnode;
}

/*----------------------------------------------FUNCTIONS FOR TREE NAME NODE----------------------------------------------*/
//creates a new tree name node
tree_name_node *create_tree_name_node()
{
	tree_name_node *namenode = malloc(sizeof(tree_name_node));
	strcpy(namenode-> treeName, "");
	namenode-> left = NULL;
	namenode-> right = NULL;
	namenode-> theTree = NULL;

	return namenode;
}

//prints the tree names at the top of the ouput
void print_tree_names(FILE *ofp, tree_name_node *namenode) 
{
  	if(namenode != NULL)
	{
    	print_tree_names(ofp, namenode-> left);
  		fprintf(ofp, "%s ", namenode-> treeName);
  		print_tree_names(ofp, namenode-> right);
	}	
	else 
	{
		return;
	}
}

//same thing as last insertion but for tree name node
tree_name_node *binary_tree_insertion_tree(tree_name_node *root, tree_name_node *parent, tree_name_node *namenode) 
{
  	if(root == NULL)
	{
		return namenode;
	}

  	if(strcmp(namenode-> treeName, parent-> treeName) > 0) 
	{
    	if(parent-> right == NULL)
		{
			parent-> right = namenode;
		}
    	else
		{
			return binary_tree_insertion_tree(root, parent-> right, namenode);
		}
  	}
  	else 
	{
    	if(parent-> left == NULL)
		{
			parent-> left = namenode;
		}
    	else
		{
			return binary_tree_insertion_tree(root, parent-> left, namenode);
		}
  	}
  	return root;
}

//finds the minimum tree name node
tree_name_node *smoll_tree_name(tree_name_node *namenode) 
{
  	if(namenode == NULL)
	{
		return NULL;
	}
  	
	if(namenode-> left == NULL)
	{
		return namenode;
	}
	else
	{
	  	return smoll_tree_name(namenode-> left);
	}
}

//deletes tree name node
tree_name_node *delete_tree_name_node(tree_name_node *namenode, char *tree_name) 
{
	tree_name_node* minimum;
	tree_name_node* temp;

	if(namenode == NULL)
	{
		return NULL;
	}
  	else if(strcmp(tree_name, namenode-> treeName) > 0)
	{
		namenode-> right = delete_tree_name_node(namenode-> right, tree_name);
	}
	else if(strcmp(tree_name, namenode-> treeName) < 0)
	{
		namenode-> left = delete_tree_name_node(namenode-> left, tree_name);
	}
  	else 
	{
    	if (namenode-> right == NULL) 
		{
			temp = namenode->left;
      		return temp;
    	}
		else if (namenode-> left == NULL) 
		{
      		temp = namenode-> right;
      		return temp;
    	}    	
    	else 
		{
      		minimum = smoll_tree_name(namenode-> right);
      		strcpy(namenode-> treeName, minimum-> treeName); 
      		namenode-> right = delete_tree_name_node(namenode-> right, minimum-> treeName);
    	}
  	}
  	return namenode;
}

/*----------------------------------------------GENERAL PURPOSE FUNCTIONS----------------------------------------------*/
//required function that aids in printing in the right format
void traverse_in_order(FILE *ofp, tree_name_node *namenode) 
{
  	if (namenode != NULL)
	{
		traverse_in_order(ofp, namenode-> left);
  		fprintf(ofp, "===%s===\n", namenode-> treeName);

  		print_item_names(ofp, namenode-> theTree);
  		fprintf(ofp, "\n");

  		traverse_in_order(ofp, namenode-> right);
	}
}

//used by the command function to return the correct count of items before item
int find_items_before(item_node *itemnode, char *item_name, int *frog) 
{
	int righty = 0; 
	int lefty = 0; 

  	if (itemnode == NULL)
	{
		return 0;
	}
  	
	lefty = find_items_before(itemnode-> left, item_name, frog);
	if (*frog)
	{
		return lefty;
	}
  	if (strcmp(itemnode-> name, item_name) == 0) 
	{
		(*frog) = 1;
    	return lefty;
  	}
	righty = find_items_before(itemnode-> right, item_name, frog);

  	return lefty + righty + 1;
}

//gets the height, used by the command function
int find_height(item_node *itemnode) 
{
	int righty = 0;
	int lefty = 0;

  	if (itemnode == NULL)
	{
		return -1;
	}
  	if (itemnode-> right == NULL && itemnode-> left == NULL)
	{
		return 0;
	}

  	lefty = find_height(itemnode-> left);
  	righty = find_height(itemnode-> right);
  	if (righty > lefty)
	{
		lefty = righty;
	}
  	return lefty + 1;
}

//used by count command to find the correct count number
int count_it_up(item_node *itemnode) 
{
	int righty = 0;
	int lefty = 0;

  	if (itemnode == NULL)
	{
		return 0;
	}
	else 
	{
		righty = count_it_up(itemnode-> right);
		lefty = count_it_up(itemnode-> left);
	
  		return lefty + righty + (itemnode-> count);
	}
}


/*----------------------------------------------COMMAND FUNCTIONS----------------------------------------------*/
//stupid search thing that i hate with a burning passion
void search_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	char item_name[32];
	char tree_name[32];
	item_node *itemnode;
  	
  	fscanf(ifp, "%s %s\n", tree_name, item_name);
  	itemnode = search_all(ofp, root, tree_name, item_name);
 	if (itemnode != NULL)
	{
		fprintf(ofp, "%d %s found in %s\n", itemnode-> count, item_name, tree_name);
	}
}

//command that will call on correct functions and return: items before
void items_before_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	char item_name[32];
	char tree_name[32];
	int here = 0;
	int before = 0;
	tree_name_node *namenode;

  	fscanf(ifp, "%s %s\n", tree_name, item_name);
  	namenode = search_for_name_node(root, tree_name);
  	if (namenode != NULL) 
	{
    	before = find_items_before(namenode-> theTree, item_name, &here);
    	fprintf(ofp, "item before %s: %d\n", item_name, before);
  	}
}

//command that will call on correct functions and return: height information
void height_function_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	//char item_name[32];
	char tree_name[32];
	char *result = "";
	int righty = 0;
	int lefty = 0;
	int difference = 0;
  	
  	fscanf(ifp, "%s\n", tree_name);
  	tree_name_node *namenode = search_for_name_node(root, tree_name);
  	if (namenode != NULL) 
	{
    	righty = find_height(namenode-> theTree-> right);
    	lefty = find_height(namenode-> theTree-> left);
    	difference = abs(righty - lefty);
    	
		if(difference == 0)
		{
			result = "balanced";
		}
    	else
		{
			result = "not balanced";
		}
    	fprintf(ofp, "%s: left height %d, right height %d, difference %d, %s\n", tree_name, lefty, righty, difference, result);
  	}
}

//command that will call on correct functions and return: the count of an item
void count_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	//char item_name[32];
	char tree_name[32];
	tree_name_node *namenode;
	int count = 0;
  	
  	fscanf(ifp, "%s\n", tree_name);
  	namenode = search_for_name_node(root, tree_name);
  	count = count_it_up(namenode-> theTree);

  	fprintf(ofp, "%s count %d\n", tree_name, count);
}

//command that will call on correct functions and return: nothing, it destorys an item from a tree
void delete_item_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	char item_name[32];
	char tree_name[32];
	tree_name_node *namenode;
  	
  	fscanf(ifp, "%s %s\n", tree_name, item_name);
  	namenode = search_for_name_node(root, tree_name);
  	if (namenode != NULL) 
	{
    	namenode-> theTree = delete_item_node(namenode-> theTree, item_name);
    	fprintf(ofp, "%s deleted from %s\n", item_name, tree_name);
  	}
}

//command that will call on correct functions and return: nothing, removes a tree
tree_name_node *delete_tree_command(FILE *ifp, FILE *ofp, tree_name_node *root) 
{
  	//char item_name[32];
	char tree_name[32];
  	
  	fscanf(ifp, "%s\n", tree_name);
  	fprintf(ofp, "%s deleted\n", tree_name);
  	
	return delete_tree_name_node(root, tree_name);
}

//function that calls on all of the commands and runs them in the appropriate order
void execute_commands(FILE *ifp, FILE *ofp, int numCommands, tree_name_node *root)
{
	int i = 0;

	fprintf(ofp, "=====Processing Commands=====\n");
	for(i = 0; i < numCommands; i++)
	{
    	char command[32];
    	fscanf(ifp, "%s ", command);

    	if (strcmp(command, "search") == 0) 
		{
        	search_command(ifp, ofp, root);
    	}
    	else if (strcmp(command, "item_before") == 0) 
		{
        	items_before_command(ifp, ofp, root);
    	}
    	else if (strcmp(command, "height_balance") == 0) 
		{
        	height_function_command(ifp, ofp, root);
    	}
    	else if (strcmp(command, "count") == 0) 
		{
        	count_command(ifp, ofp, root);
    	}
    	else if (strcmp(command, "delete") == 0) 
		{
        	delete_item_command(ifp, ofp, root);
    	}
    	else if (strcmp(command, "delete_tree") == 0) 
		{
        	root = delete_tree_command(ifp, ofp, root);
    	}
    }
}

/*----------------------------------------------MAIN----------------------------------------------*/
int main(void) 
{	
	FILE *ifp;
	FILE *ofp;
	int i = 0;
	int nitems = 0;
	int ntrees = 0;
	int ncommands = 0;
	
	ifp = fopen("cop3502-as4-input.txt", "r");
  	ofp = fopen("cop3502-as4-output-Schrock-Noah.txt", "w+");

    fscanf(ifp, "%d %d %d\n", &ntrees, &nitems, &ncommands);

    tree_name_node *root = NULL;
    for (int i = 0; i < ntrees; i++) 
	{
      	tree_name_node *namenode;
		
		namenode = create_tree_name_node();
      	fscanf(ifp, "%s\n", namenode-> treeName);
      	root = binary_tree_insertion_tree(root, root, namenode);
    }

    for (int i = 0; i < nitems; i++) 
	{
      	item_node *itemnode;
		char parent_name[32]; 

		itemnode = create_item_node();
      	fscanf(ifp, "%s %s %d\n", parent_name, itemnode-> name, &itemnode-> count, NULL, NULL);
      	insert(root, parent_name, itemnode);
    }
    print_tree_names(ofp, root);
	fprintf(ofp, "\n");
    traverse_in_order(ofp, root);
	execute_commands(ifp, ofp, ncommands, root);

    fclose(ifp);
    fclose(ofp);   
  	return 0;
}