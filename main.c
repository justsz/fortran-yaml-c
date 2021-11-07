#include <yaml.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TypeNode TypeNode;

const int STRING_LENGTH = 1024;

struct TypeNode
{
  // Node
  int type;
  
  // Scalar
  char *string;
  
  // Dictionary
  TypeNode *first_keyvaluepair;
  char *key;
  TypeNode *value;
  TypeNode *next_keyvaluepair;
  
  // List
  TypeNode *first_listitem;
  TypeNode *node;
  TypeNode *next_listitem;

};


// doesn't print styles.
TypeNode* read_value(yaml_document_t *document_p, yaml_node_t *node)
{

	yaml_node_t *next_node_p;
  TypeNode *mynode;

	switch (node->type) {
		case YAML_NO_NODE:
			mynode = (TypeNode*) malloc(sizeof(TypeNode));
      mynode->type = 4;
			break;
		case YAML_SCALAR_NODE:
      mynode = (TypeNode*) malloc(sizeof(TypeNode));
      mynode->type = 3;
      mynode->string = (char*) malloc(STRING_LENGTH * sizeof(char));
      strcpy(mynode->string, node->data.scalar.value);
			break;
		case YAML_SEQUENCE_NODE:
      mynode = (TypeNode*) malloc(sizeof(TypeNode));
      mynode->type = 2;
      mynode->first_listitem = (TypeNode*) malloc(sizeof(TypeNode));
    
			yaml_node_item_t *i_node;
      TypeNode *listitem = mynode->first_listitem;
			for (i_node = node->data.sequence.items.start; i_node < node->data.sequence.items.top; i_node++) {
				next_node_p = yaml_document_get_node(document_p, *i_node);
				listitem->node = read_value(document_p, next_node_p);
        if (i_node < node->data.sequence.items.top - 1){
          listitem->next_listitem = (TypeNode*) malloc(sizeof(TypeNode));
          listitem = listitem->next_listitem;
        }
      }
			break;
		case YAML_MAPPING_NODE:
      
			mynode = (TypeNode*) malloc(sizeof(TypeNode));
      printf("HERE%i\n",mynode);
      
      mynode->type = 1;
      mynode->first_keyvaluepair = (TypeNode*) malloc(sizeof(TypeNode));
      
      TypeNode *keyvaluepair = mynode->first_keyvaluepair;
			yaml_node_pair_t *i_node_p;
			for (i_node_p = node->data.mapping.pairs.start; i_node_p < node->data.mapping.pairs.top; i_node_p++) {
				
        keyvaluepair->key = (char*) malloc(STRING_LENGTH * sizeof(char));
        next_node_p = yaml_document_get_node(document_p, i_node_p->key);
        strcpy(keyvaluepair->key, next_node_p->data.scalar.value);

				next_node_p = yaml_document_get_node(document_p, i_node_p->value);
				keyvaluepair->value = read_value(document_p, next_node_p);
        
        if (i_node_p < node->data.mapping.pairs.top - 1){
          keyvaluepair->next_keyvaluepair = (TypeNode*) malloc(sizeof(TypeNode));
          keyvaluepair = keyvaluepair->next_keyvaluepair;
        }
        
			}
			break;
      
    }  
  
  return mynode;
}

TypeNode* LoadFile_c(const char *file_name)
{
	yaml_parser_t parser;
	yaml_document_t document;
	int error = 0;
  TypeNode* root;
  

	// printf("Loading '%s': \n", file_name);

	FILE *file = fopen(file_name, "rb");
	assert(file);

	assert(yaml_parser_initialize(&parser));

	yaml_parser_set_input_file(&parser, file);

	int done = 0;

	if (!yaml_parser_load(&parser, &document)) {
		fprintf(stderr, "Failed to load document in %s\n", file_name);
		error = 1;
	}

	done = (!yaml_document_get_root_node(&document));


  root = read_value(&document, yaml_document_get_root_node(&document));
  
	yaml_document_delete(&document);


	yaml_parser_delete(&parser);

	assert(!fclose(file));
  
  return root;
}

void tester(int* a){
  
  a = (int*) malloc(5*sizeof(int)); 
  
}


int main()
{

  TypeNode *root = NULL;
  root = LoadFile_c("../test.yaml");
  printf("%i\n",root);
  
  int *a;
  tester(a);
  printf("%i\n",a);
  
  return 0;
}