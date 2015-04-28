/******************************************************************************
*                                                                             *
*   File Name   : ini.h           *
*   Author      : greenet Administrator           *
*   Create Date : 2015-2-12           *
*   Version     : 1.0           *
*   Function    : .           *
*                                                                             *
******************************************************************************/

#ifndef INI_H_
#define INI_H_

#include <stdio.h>

typedef struct ini_comment
{
    char *desc;
    struct ini_comment *pre;
    struct ini_comment *next;
}ini_comment_t;

typedef struct ini_item
{
    char *name;
    char *value;
    ini_comment_t *comments;
    struct ini_item *pre;
    struct ini_item *next;
}ini_item_t;

typedef struct ini_selection
{
    char *selection;
    ini_comment_t *comments;
    ini_item_t *ini_items;
    struct ini_selection *pre;
    struct ini_selection *next;
}ini_selection_t;

typedef struct ini_file
{
    void *fd;
    ini_selection_t *selections;
}ini_file_t;



/*
 *   Author      : dengchao
 *   Create Date : 2014-12-12
 *   Input Args  : path - the absolute path and filename of the ".ini" configure file
 *   Output Args :
 *   Return      : return the fd of the ".ini" configure file
 *   History     :
 */
ini_file_t* read_ini(const char *path);

/*
 *   Author      : dengchao
 *   Create Date : 2014-12-12
 *   Input Args  : ini_file - the ".ini" file pointer
 *   Output Args :
 *   Return      :
 *   History     :
 */
void close_ini(ini_file_t **ini_file);

/*
 *   Author      : dengchao
 *   Create Date : 2014-12-12
 *   Input Args  : ini_file - the ".ini" file pointer
 *                 selection - the name of the selection
 *                 item - the name of the item
 *   Output Args :
 *   Return      :
 *   History     :
 */
char *get_value(const ini_file_t *ini_file, const char *selection, const char *item);

/*
 *   Author      : dengchao
 *   Create Date : 2014-12-12
 *   Input Args  : ini_file - the ".ini" file pointer
 *                 path - the absolute path and filename of the ".ini" configure file
 *   Output Args :
 *   Return      : 0: succeed; !0: failed
 *   History     :
 */
int save_ini(const ini_file_t *ini_file, const char *path);

#endif /* INI_H_ */