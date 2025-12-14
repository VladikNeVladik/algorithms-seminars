#ifndef HEADER_GUARD_TREE_RB_H_INCLUDED
#define HEADER_GUARD_TREE_RB_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

//==================//
// Структура данных //
//==================//

// Тип Node_t - идентификатор узла дерева
typedef uint32_t Node_t;

// Макроопределение NULL_NODE - идентификатор узла-пустышки
#define NULL_NODE ((Node_t) 0xFFFFFFFFU)

// Тип TreeNode - узел красно-чёрного дерева.
struct TreeNode {
    // Идентификатор родительского узла.
    // Для корневой вершины равен NULL_NODE.
    Node_t parent_id;
    // Идентификатор левого дочернего узла.
    // В случае отсутствия левого дочернего узла равен NULL_NODE.
    Node_t left_id;
    // Идентификатор правого дочернего узла.
    // В случае отсутствия правого дочернего узла равен NULL_NODE.
    Node_t right_id;

    // Ключ узла дерева.
    Key_t key;
    // Значение узла дерева.
    Value_t value;

    // Цвет узла в красно-чёрном дереве.
    bool is_black;
};

typedef struct TreeNode TreeNode;

// Тип Tree - красно-чёрное дерево поиска.
typedef struct {
    // Динамический массив узлов дерева.
    // Идентификатор узла дерева равен индексу этого узла в массиве nodes.
    TreeNode* nodes;
    // Счётчик узлов двоичного дерева.
    size_t size;
    // Размер массива nodes.
    size_t capacity;

    // Корневой узел двоичного дерева.
    Node_t root_id;
} Tree;

// Предварительная декларация функции для печати.
void tree_print(Tree* tree);

//======================//
// Управление ресурсами //
//======================//

//==================================================================================================
// Функция: tree_alloc
// Назначение: Инициализирует красно-чёрное дерево
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree (in/out) - дерево, которое требуется инициализировать.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Для каждого дерева, инициализируемого с помощью tree_alloc,
//   должна быть вызвана функция tree_free.
//==================================================================================================
RetCode tree_alloc(Tree* tree)
{
    // Инициализируем поля структуры.
    tree->size     = 0U;
    tree->capacity = 1U;
    tree->root_id  = NULL_NODE;

    // Выделяем память для массива узлов дерева.
    tree->nodes = calloc(tree->capacity, sizeof(TreeNode));
    if (tree->nodes == NULL)
    {
        return RET_NOMEM;
    }

    return RET_OK;
}

//==================================================================================================
// Функция: tree_free
// Назначение: Освобождает ресурсы красно-чёрного дерева
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree (in/out) - дерево, ресурсы которого требуется освободить.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Для каждого дерева, освобождаемого с помощью tree_free,
//   должна ранее быть вызвана функция tree_alloc.
//==================================================================================================
RetCode tree_free(Tree* tree)
{
    // Освобождаем ранее выделенную динамическую память.
    free(tree->nodes);

    return RET_OK;
}

//=========================//
// Вспомогательные функции //
//=========================//

//==================================================================================================
// Функция: tree_get
// Назначение: Возвращает узел дерева по идентификатору узла дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор узла дерева.
//
// Возвращаемое значение:
// Указатель на узел дерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Идентификатор node_id - индекс, не выходящий за границы массива tree->nodes.
//==================================================================================================
TreeNode* tree_get(Tree* tree, Node_t node_id)
{
    // Возвращаем узел в массиве по индексу, равному идентификатору узла.
    return &tree->nodes[node_id];
}

//==================================================================================================
// Функция: tree_transplant
// Назначение: Производит замену одного поддерева на другое поддерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree            (in) - бинарное дерево поиска.
// transplanted_id (in) - идентификатор корневого узла заменяемого поддерева.
// new_id          (in) - идентификатор коренового узла поддерева, .
//
// Возвращаемое значение:
// отсутствует
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Описание алгоритма можно найти в книге Introduction to Algorithms (Cormen, Leiserson, Rivest,
//   Stein), в части 12.3 третьего издания.
// - Балансировка дерева производится на более высоком уровне реализации.
//==================================================================================================
void tree_transplant(Tree* tree, Node_t transplanted_id, Node_t new_id)
{
    // Указатель на заменяемый узел дерева.
    TreeNode* transplanted = tree_get(tree, transplanted_id);

    if (transplanted->parent_id == NULL_NODE)
    {   // Заменяемый узел является корневым.
        // Заменяем идентификатор корневого узла дерева.
        tree->root_id = new_id;
    }
    else
    {   // Заменяемый узел не является корневым.
        // Указатель на родительский узел для заменяемого дерева.
        TreeNode* parent = tree_get(tree, transplanted->parent_id);
        if (transplanted_id == parent->left_id)
        {   // Заменяемый узел находится в левом поддереве своего родительского узла.
            parent->left_id = new_id;
        }
        else
        {   // Заменяемый узел находится в правом поддереве своего родительского узла.
            parent->right_id = new_id;
        }
    }

    if (new_id != NULL_NODE)
    {   // Новый узел не является узлом-пустышкой.
        // Указатель на новый узел.
        TreeNode* new = tree_get(tree, new_id);

        // Обновляем идентификатор родительского узла для нового узла.
        new->parent_id = transplanted->parent_id;
    }
}

//==================================================================================================
// Функция: tree_node_allocate
// Назначение: Выделяет новый узел дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree (in) - красно-чёрное дерево поиска.
//
// Возвращаемое значение:
// Идентификатор нового узла.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
Node_t tree_node_allocate(Tree* tree)
{
    // Проверяем наличие невыделенных узлов дерева.
    if (tree->size == tree->capacity)
    {   // Невыделенные узлы отсутствуют.

        // Новый размер массива узлов.
        size_t new_capacity =
            (tree->size == 0U)? 1U : (2U * tree->capacity);

        // Перевыделяем массив узлов дерева.
        tree->nodes = realloc(tree->nodes, new_capacity * sizeof(TreeNode));
        if (tree->nodes == NULL)
        {
            return NULL_NODE;
        }

        // Обновляем размер массива узлов.
        tree->capacity = new_capacity;
    }

    // Выделяем новый узел на первом свободном месте в массиве
    Node_t allocated_id = tree->size;

    // Увеличиваем счётчик выделенных узлов.
    tree->size += 1U;

    // Указатель на новый узел.
    TreeNode* allocated = tree_get(tree, allocated_id);

    // Инициализируем узел как отвязанный от дерева.
    allocated->parent_id = NULL_NODE;
    allocated->left_id   = NULL_NODE;
    allocated->right_id  = NULL_NODE;

    // По умолчанию выделяем красный узел.
    allocated->is_black = false;

    return allocated_id;
}

//==================================================================================================
// Функция: tree_node_free
// Назначение: Освбождает узел дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree     (in) - красно-чёрное дерево поиска.
// freed_id (in) - идентификатор узла для освобождения.
//
// Возвращаемое значение:
// отсутствует.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Балансировка дерева производится на более высоком уровне реализации.
//==================================================================================================
void tree_node_free(Tree* tree, Node_t freed_id)
{
    // Удаляемый из дерева узел.
    TreeNode* freed = tree_get(tree, freed_id);

    // Идентификатор последнего узла в массиве узлов
    Node_t last_id = tree->size - 1U;

    if (freed_id != last_id)
    {   // Удаляемый узел не последний в массиве узлов.

        // Последний узел.
        TreeNode* last = tree_get(tree, last_id);

        // Переносим последний узел в массиве узлов в освободившееся пространство.
        *freed = *last;
        tree_transplant(tree, last_id, freed_id);

        // Обновляем родителя правого и левого дочерних узлов.
        if (freed->left_id != NULL_NODE)
        {
            TreeNode* left = tree_get(tree, freed->left_id);
            left->parent_id = freed_id;
        }
        if (freed->right_id != NULL_NODE)
        {
            TreeNode* right = tree_get(tree, freed->right_id);
            right->parent_id = freed_id;
        }
    }

    // Уменьшаем счётчик выделенных узлов
    tree->size -= 1U;
}

//==================================================================================================
// Функция: tree_search_node
// Назначение: Производит поиск узла в дереве.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree       (in)  - красно-чёрное дерево поиска.
// search_key (in)  - ключ, по которому производится поиск.
// parent_id  (out) - указатель на идентификатор родительского узла.
//
// Возвращаемое значение:
// Узел по ключу или NULL_NODE в случае отсутствия узла.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
Node_t tree_search_node(Tree* tree, Key_t search_key, Node_t* parent_id)
{
    // Родительский узел корневого узла - это узел-пустышка.
    *parent_id = NULL_NODE;

    // Тукущий рассматриваемый идентификатор.
    Node_t cur_id = tree->root_id;
    // Обходим дерево от корня к листьям.
    while (cur_id != NULL_NODE)
    {
        // Текущий рассматриваемый узел.
        TreeNode* node = tree_get(tree, cur_id);

        if (search_key == node->key)
        {   // Текущий рассматриаемый узел имеет подходящий ключ.
            return cur_id;
        }

        // Обновляем идентификатор рассматриваемого узла.
        *parent_id = cur_id;

        if (search_key < node->key)
        {   // Если искомый ключ меньше текущего, то рассматриваем левое поддерево.
            cur_id = node->left_id;
        }
        else
        {   // Если искомый ключ больше текущего, то рассматриваем правое поддерево.
            cur_id = node->right_id;
        }
    }

    // В случае неуспеха возвращаем идентификатор узла-пустышки.
    return NULL_NODE;
}


//==================================================================================================
// Функция: tree_minimum
// Назначение: Ищет узел с минимальным ключом в в заданном поддереве.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree       (in) - красно-чёрное дерево.
// subtree_id (in) - идентификатора поддерева для поиска минимума.
//
// Возвращаемое значение:
// Идентификатор узла с минимальным ключом.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Описание алгоритма можно найти в книге Introduction to Algorithms (Cormen, Leiserson, Rivest,
//   Stein), в части 12.3 третьего издания.
//==================================================================================================
Node_t tree_minimum(Tree* tree, Node_t subtree_id)
{
    // Текущий рассматривамый узел.
    TreeNode* subtree = tree_get(tree, subtree_id);

    // В цикле итеративно переходим к левому дочернему узлу.
    while (subtree->left_id != NULL_NODE)
    {
        subtree_id = subtree->left_id;
        subtree = tree_get(tree, subtree_id);
    }

    return subtree_id;
}

//==================================================================================================
// Функция: tree_rotate_left
// Назначение: производит левый поворот над заданной вершиной дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - красно-чёрное дерево.
// node_id (in) - валидный идентификатор корневого узла поддерева.
//
// Возвращаемое значение:
// Идентификатор нового корневого узла поддерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Визуализация преобразования, заданного данной функцией.
//   Здесь узел node задан идентификатором node_id, а узел ret - ret_id.
//    node                     ret
//    / \                      / \    *
//   T1 ret     ------->     node T3
//      / \                  / \      *
//     T2  T3               T1  T2
//
// - Узел node должен иметь правый дочерний узел.
// - Операция не производит перерассчёта высот для родительских узлов.
//==================================================================================================
Node_t tree_rotate_left(Tree* tree, Node_t node_id)
{
    // Текущий корневой узел поддерева.
    TreeNode* node = tree_get(tree, node_id);

    // Идентификатор нового корневого узла поддерева.
    Node_t ret_id = node->right_id;
    // Новый корневой узел поддерева.
    TreeNode* ret = tree_get(tree, ret_id);

    // Идентификатор текущего левого дочернего узла для узла ret.
    Node_t t2_id = ret->left_id;

    // Идентификатор родительского узла для узла node.
    Node_t parent_id = node->parent_id;

    // Обновляем связи узла node.
    node->parent_id = ret_id;
    node->right_id  = t2_id;

    // Обновляем связи узла ret.
    ret->left_id    = node_id;
    ret->parent_id  = parent_id;

    // Обновляем идентификатор родителя для узла T2.
    if (t2_id != NULL_NODE)
    {
        TreeNode* t2 = tree_get(tree, t2_id);
        t2->parent_id = node_id;
    }

    // Обновляем идентификатор дочернего узла для родителя узла node.
    if (parent_id == NULL_NODE)
    {
        tree->root_id = ret_id;
    }
    else
    {
        TreeNode* parent = tree_get(tree, parent_id);

        if (parent->left_id == node_id)
        {
            parent->left_id = ret_id;
        }
        else
        {
            parent->right_id = ret_id;
        }
    }

    return ret_id;
}

//==================================================================================================
// Функция: tree_rotate_right
// Назначение: производит правый поворот над заданной вершиной дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - красно-чёрное дерево.
// node_id (in) - валидный идентификатор корневого узла поддерева.
//
// Возвращаемое значение:
// Идентификатор нового корневого узла поддерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Визуализация преобразования, заданного данной функцией.
//   Здесь узел node задан идентификатором node_id, а узел ret - ret_id.
//      node                ret
//      / \                 / \      *
//    ret  T3   ------->   T1 node
//    / \                     / \    *
//   T1  T2                  T2  T3
//
// - Узел node должен иметь левый дочерний узел.
// - Операция не производит перерассчёта высот для родительских узлов.
//==================================================================================================
Node_t tree_rotate_right(Tree* tree, Node_t node_id)
{
    // Текущий корневой узел поддерева.
    TreeNode* node = tree_get(tree, node_id);

    // Идентификатор нового корневого узла поддерева.
    Node_t ret_id = node->left_id;
    // Новый корневой узел поддерева.
    TreeNode* ret = tree_get(tree, ret_id);

    // Идентификатор текущего правого дочернего узла для узла ret.
    Node_t t2_id = ret->right_id;

    // Идентификатор родительского узла для узла node.
    Node_t parent_id = node->parent_id;

    // Обновляем связи узла node.
    node->parent_id = ret_id;
    node->left_id = t2_id;

    // Обновляем связи узла ret.
    ret->parent_id = parent_id;
    ret->right_id  = node_id;

    // Обновляем идентификатор родителя для узла T2.
    if (t2_id != NULL_NODE)
    {
        TreeNode* t2 = tree_get(tree, t2_id);
        t2->parent_id = node_id;
    }

    // Обновляем идентификатор дочернего узла для родителя узла node.
    if (parent_id == NULL_NODE)
    {
        tree->root_id = ret_id;
    }
    else
    {
        TreeNode* parent = tree_get(tree, parent_id);

        if (parent->left_id == node_id)
        {
            parent->left_id = ret_id;
        }
        else
        {
            parent->right_id = ret_id;
        }
    }

    return ret_id;
}

// Максимальная глубина печати дерева
#define MAX_PRINT_DEPTH 40U

// Цвета для печати.
#define BRED    "\033[1;31m"
#define BCYAN   "\033[1;36m"
#define RESET   "\033[0m"

//==================================================================================================
// Функция: tree_print_recursive
// Назначение: Рекурсивно производит печать поддерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree   (in) - красно-чёрное дерево поиска.
// node_i (in) - идентификатор поддерева для печати.
// level  (in) - обрабатываемый уровень дерева.
// state  (in) - состояние печати дерева.
//
// Возвращаемое значение:
// отсутствует.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
void tree_print_recursive(Tree* tree, Node_t node_i, size_t level, int state[MAX_PRINT_DEPTH])
{
    if (level == MAX_PRINT_DEPTH)
    {
        // Печать уровня с номером более чем MAX_PRINT_DEPTH не производится.
        return;
    }

    // Текущий узел.
    TreeNode* node = (node_i == NULL_NODE)? NULL : tree_get(tree, node_i);

    // Рекурсивно печатаем левое поддерево.
    state[level] = 0;
    if (node != NULL)
    {
        tree_print_recursive(tree, node->right_id, level + 1U, state);
    }
    state[level] = 1;

    // Печатаем строку текущего узла.
    for (size_t lvl = 0U; lvl + 1U < level; lvl++)
    {
        switch (state[lvl])
        {
            case 0:
                printf("      ");
                break;
            case 1:
                printf(" │    ");
                break;
            case 2:
                printf(" │    ");
                break;
            case 3:
                printf("      ");
                break;
            default: break;
        }
    }

    if (level != 0U)
    {
        if (state[level - 1U] == 0)
        {
            state[level - 1U] = 1;
        }

        switch (state[level - 1U])
        {
            case 1:
                printf(" ┌────");
                break;
            case 2:
                printf(" └────");
                break;
            case 3:
                printf("      ");
                break;
            default: break;
        }

        if (state[level - 1U] == 2)
        {
            state[level - 1U] = 3;
        }
    }

    // Печатаем текущий узел.
    if (node == NULL)
    {
        printf("0\n");
    }
    else
    {
        if (node->is_black)
        {
            printf(BCYAN "[%d]\n" RESET, node->key);
        }
        else
        {
            printf(BRED "[%d]\n" RESET, node->key);
        }
    }

    // Рекурсивно печатаем правое поддерево.
    state[level] = 2;
    if (node != NULL)
    {
        tree_print_recursive(tree, node->left_id, level + 1U, state);
    }
    state[level] = 3;
}

//==================================================================================================
// Функция: tree_insert_fixup
// Назначение: Производит перебалансировку красно-чёрного дерева при вставке узла.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор узла, вставляемого в дерево.
//
// Возвращаемое значение:
// отсутствует
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Описание алгоритма можно найти в книге Introduction to Algorithms (Cormen, Leiserson, Rivest,
//   Stein), в части 13.3 третьего издания.
//==================================================================================================
void tree_insert_fixup(Tree* tree, Node_t node_id)
{
    tree_print(tree);
    printf("\n");
    sleep(1);
    
    // Указатель на текущйи узел.
    TreeNode* node = tree_get(tree, node_id);
    // Идентификатор родительского узла для текущего узла.
    Node_t parent_id = node->parent_id;

    while (parent_id != NULL_NODE)
    {
        // Указатель на родительский узел.
        TreeNode* parent = tree_get(tree, parent_id);
        if (parent->is_black)
        {   // Родительский узел чёрный.
            break;
        }

        // Родительский узел родительского узла.
        Node_t grandparent_id = parent->parent_id;
        if (grandparent_id == NULL_NODE)
        {   // Процесс балансировки дошёл до корня дерева.
            break;
        }
        // Указатель на родительский узел родительского узла.
        TreeNode* grandparent = tree_get(tree, grandparent_id);

        if (parent_id == grandparent->left_id)
        {   // Мы находимся в ситуации:
            //      grandparent
            //       /     \     *
            //    parent   uncle
            //      |  
            //     node

            // Другой ребёнок родителя нашего родителя.
            Node_t uncle_id = grandparent->right_id;

            if (uncle_id != NULL_NODE && tree_get(tree, uncle_id)->is_black == false)
            {   // Узел-дядя является красным.
                TreeNode* uncle = tree_get(tree, uncle_id);

                // Перекрашиваем узлы дерева. 
                parent->is_black = true;
                uncle->is_black = true;
                grandparent->is_black = false;

                // Переходим к следующему узлу.
                node_id = grandparent_id;
                node = grandparent;

                parent_id = node->parent_id;
            }
            else
            {   // Узел-дядя отсутствует или является чёрным.
                if (node_id == parent->right_id)
                {
                    node_id = parent_id;
                    node = parent;

                    // Производим левый поворот дерева.
                    tree_rotate_left(tree, node_id);

                    parent_id = node->parent_id;
                    parent = tree_get(tree, parent_id);
                }

                parent->is_black = true;
                grandparent->is_black = false;

                // Производим правый поворот дерева.
                tree_rotate_right(tree, grandparent_id);
            }
        }
        else
        {   // Мы находимся в ситуации:
            //      grandparent 
            //       /     \     *
            //    parent   uncle
            //      |  
            //     node

            // Другой ребёнок родителя нашего родителя.
            Node_t uncle_id = grandparent->left_id;

            if (uncle_id != NULL_NODE && tree_get(tree, uncle_id)->is_black == false)
            {   // Узел-дядя является красным.
                TreeNode* uncle = tree_get(tree, uncle_id);

                // Перекрашиваем узлы дерева. 
                parent->is_black = true;
                uncle->is_black = true;
                grandparent->is_black = false;

                // Переходим к следующему узлу.
                node_id = grandparent_id;
                node = grandparent;

                parent_id = node->parent_id;
            }
            else
            {   // Узел-дядя отсутствует или является чёрным.
                if (node_id == parent->left_id)
                {
                    node_id = parent_id;
                    node = parent;

                    // Производим левый поворот дерева.
                    tree_rotate_right(tree, node_id);

                    parent_id = node->parent_id;
                    parent = tree_get(tree, parent_id);
                }

                parent->is_black = true;
                grandparent->is_black = false;

                // Производим правый поворот дерева.
                tree_rotate_left(tree, grandparent_id);
            }
        }

        tree_print(tree);
        printf("\n");
        sleep(1);
    }

    // Раскрашиваем корневой узел дерева в чёрный.
    tree_get(tree, tree->root_id)->is_black = true;
}

//==================================================================================================
// Функция: tree_remove_fixup
// Назначение: Производит перебалансировку красно-чёрного дерева при вставке узла.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - красно-чёрное дерево.
// node_id (in) - идентификатор узла, с которого требуется начать перебалансировку.
//
// Возвращаемое значение:
// отсутствует
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Описание алгоритма можно найти в книге Introduction to Algorithms (Cormen, Leiserson, Rivest,
//   Stein), в части 13.4 третьего издания.
//==================================================================================================
void tree_remove_fixup(Tree* tree, Node_t node_id, Node_t parent_id)
{
    while (parent_id != NULL_NODE && (node_id == NULL_NODE || tree_get(tree, node_id)->is_black))
    {
        // Указатель на родительский узел.
        TreeNode* parent = tree_get(tree, parent_id);

        if (node_id == parent->left_id)
        {   // Текущий узел является левым дочерним узлом для своего родителя.
            /*    parent
             *     /   \
             *  node  sibling
             */
            // Идентификатор братского узла для текущего узла.
            // Т.к. на правом и левом путях кол-во чёрных узлов одинаковое, то этот узел ненулевой.
            Node_t sibling_id = parent->right_id;
            // Братский узел для текущего узла.
            TreeNode* sibling = tree_get(tree, sibling_id);

            if (!sibling->is_black)
            {   // Братский узел является красным.
                // Перекрашиваем узлы.
                sibling->is_black = true;
                parent->is_black = false;

                // Производим поворот.
                tree_rotate_left(tree, parent_id);
                // Обновляем братский узел.
                sibling_id = parent->right_id;
                sibling = tree_get(tree, sibling_id);

                tree_print(tree);
                printf("\n");
                sleep(1);
            }

            if ((sibling->left_id  == NULL_NODE || tree_get(tree, sibling->left_id )->is_black) &&
                (sibling->right_id == NULL_NODE || tree_get(tree, sibling->right_id)->is_black))
            {   // Оба дочерних узла братского узла чёрные.
                // Раскрашиваем братский узел в красный цвет.
                sibling->is_black = false;
                // Переходим к обработке поворотов вокруг родительского узла.
                node_id = parent_id;
            }
            else
            {   // У братского узла есть красный дочерний узел.
                if (sibling->right_id == NULL_NODE || tree_get(tree, sibling->right_id)->is_black)
                {   // Правый племянник отсутствует или чёрный.
                    // Перекрашиваем узлы дерева.
                    tree_get(tree, sibling->left_id)->is_black = true;
                    sibling->is_black = false;

                    // Производим поворот.
                    tree_rotate_right(tree, sibling_id);
                    // Обновляем братский узел.
                    sibling_id = parent->right_id;
                    sibling = tree_get(tree, sibling_id);
                }

                // Продолжаем перекрашивать.
                sibling->is_black = parent->is_black;
                parent->is_black = true;
                tree_get(tree, sibling->right_id)->is_black = true;

                tree_rotate_left(tree, parent_id);
                // Завершаем все повороты.
                node_id = tree->root_id;
            }
        }
        else
        {   // Текущий узел является правым дочерним узлом для своего родителя.
            /*    parent
             *     /   \
             *  sibling  node
             */
            // Идентификатор братского узла для текущего узла.
            // Т.к. на правом и левом путях кол-во чёрных узлов одинаковое, то этот узел ненулевой.
            Node_t sibling_id = parent->right_id;
            // Братский узел для текущего узла.
            TreeNode* sibling = tree_get(tree, sibling_id);

            if (!sibling->is_black)
            {   // Братский узел является красным.
                // Перекрашиваем узлы.
                sibling->is_black = true;
                parent->is_black = false;

                // Производим поворот.
                tree_rotate_right(tree, parent_id);
                // Обновляем братский узел.
                sibling_id = parent->left_id;
                sibling = tree_get(tree, sibling_id);

                tree_print(tree);
                printf("\n");
                sleep(1);
            }

            if ((sibling->right_id == NULL_NODE || tree_get(tree, sibling->right_id)->is_black) &&
                (sibling->left_id  == NULL_NODE || tree_get(tree, sibling->left_id )->is_black))
            {   // Оба дочерних узла братского узла чёрные.
                // Раскрашиваем братский узел в красный цвет.
                sibling->is_black = false;
                // Переходим к обработке поворотов вокруг родительского узла.
                node_id = parent_id;
            }
            else
            {   // У братского узла есть красный дочерний узел.
                if (sibling->left_id == NULL_NODE || tree_get(tree, sibling->left_id)->is_black)
                {   // Левый племянник отсутствует или чёрный.
                    // Перекрашиваем узлы дерева.
                    tree_get(tree, sibling->right_id)->is_black = true;
                    sibling->is_black = false;

                    // Производим поворот.
                    tree_rotate_left(tree, sibling_id);
                    // Обновляем братский узел.
                    sibling_id = parent->left_id;
                    sibling = tree_get(tree, sibling_id);
                }

                // Продолжаем перекрашивать.
                sibling->is_black = parent->is_black;
                parent->is_black = true;
                tree_get(tree, sibling->left_id)->is_black = true;

                tree_rotate_right(tree, parent_id);
                // Завершаем все повороты.
                node_id = tree->root_id;
            }
        }

        parent_id = tree_get(tree, node_id)->parent_id;

        tree_print(tree);
        printf("\n");
        sleep(1);
    }

    // Раскрашиваем последний узел.
    tree_get(tree, node_id)->is_black = true;

    tree_print(tree);
    printf("\n");
    sleep(1);
}

//============================//
// Пользовательский интерфейс //
//============================//

//==================================================================================================
// Функция: tree_search
// Назначение: Находит значение в дереве по ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree  (in)  - красно-чёрное дерево поиска.
// key   (in)  - ключ, по которому производится поиск.
// res   (out) - значение по ключу (выходной аргумент).
// found (out) - флаг успешности поиска в дереве (выходной аргумент).
//
// Возвращаемое значение:
// код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
RetCode tree_search(Tree* tree, Key_t key, Value_t* res, bool* found)
{
    if (tree == NULL || res == NULL || found == NULL)
    {
        return RET_INVAL;
    }

    Node_t parent_id = NULL_NODE;
    Node_t found_id = tree_search_node(tree, key, &parent_id);
    if (found_id == NULL_NODE)
    {
        *found = false;
        return RET_OK;
    }

    TreeNode* found_node = tree_get(tree, found_id);
    *res = found_node->value;

    *found = true;
    return RET_OK;
}

//==================================================================================================
// Функция: tree_set
// Назначение: Выставляет значение в дереве по ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree  (in) - красно-чёрное дерево.
// key   (in) - ключ, по которому производится поиск значения.
// value (in) - новое значение для заданного ключа.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
RetCode tree_set(Tree* tree, Key_t key, Value_t value)
{
    if (tree == NULL)
    {
        return RET_INVAL;
    }

    // Производим поиск значения по ключу.
    Node_t parent_id = NULL_NODE;
    Node_t found_i = tree_search_node(tree, key, &parent_id);

    // Обновляем значение уже существующего узла.
    if (found_i != NULL_NODE)
    {
        TreeNode* found = tree_get(tree, found_i);
        found->value = value;

        // Структура дерева не изменилась, перебалансировка не требуется.
        return RET_OK;
    }

    // Выделяем новый узел для несуществующего ключа.
    Node_t allocated_id = tree_node_allocate(tree);
    if (allocated_id == NULL_NODE)
    {
        // Структура дерева не изменилась, перебалансировка не требуется.
        return RET_NOMEM;
    }

    // Выставляем родителя для нового узла.
    if (tree->root_id == NULL_NODE)
    {   // Новый узел является корневым.
        tree->root_id = allocated_id;
    }
    else
    {   // Новый узел является внутренним или листовым.
        TreeNode* parent = tree_get(tree, parent_id);
        if (key < parent->key)
        {
            parent->left_id = allocated_id;
        }
        else
        {
            parent->right_id = allocated_id;
        }
    }

    // Инициализируем выделенный узел.
    TreeNode* allocated = tree_get(tree, allocated_id);

    allocated->parent_id = parent_id;
    allocated->key       = key;
    allocated->value     = value;

    // Производим перебалансировку дерева.
    tree_insert_fixup(tree, allocated_id);

    return RET_OK;
}

//==================================================================================================
// Функция: tree_remove
// Назначение: Удаляет ключ из дерева с возвратом значения.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree  (in)  - красно-чёрное дерево.
// key   (in)  - ключ, по которому производится удаление значения.
// ret   (out) - значение для ключа.
// found (out) - флаг успешности поиска ключа в дереве.
//
// Возвращаемое значение:
// код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
RetCode tree_remove(Tree* tree, Key_t key, Value_t* ret, bool* found)
{
    // Производим поиск по ключу.
    Node_t selected_parent_id = NULL_NODE;
    Node_t selected_id = tree_search_node(tree, key, &selected_parent_id);

    if (selected_id == NULL_NODE)
    {   // В случае отсутствия ключа в дереве возвращаемся из функции.
        *found = false;
        return RET_OK;
    }

    // Узел, найденный по ключу.
    TreeNode* selected = tree_get(tree, selected_id);
    // Изначальный цвет самого нижнего модифицированного узла в дереве. 
    bool modified_node_was_black = selected->is_black;
    // Идентификатор узла, с которого будет начинаться перебалансировка.
    Node_t rebalance_node_id;
    // Идентификатор родительского узла для удаляемого узла.
    Node_t rebalance_parent_id = selected_parent_id;

    if (selected->left_id == NULL_NODE)
    {   // У найденного узла отсутствует левый дочерный узел.
        tree_transplant(tree, selected_id, selected->right_id);

        // Будем выполнять балансировку с правого поддерева.
        rebalance_node_id = selected->right_id;
    }
    else if (selected->right_id == NULL_NODE)
    {   // У найденного узла отсутствует правый дочерный узел.
        tree_transplant(tree, selected_id, selected->left_id);

        // Будем выполнять балансировку с левого поддерева.
        rebalance_node_id = selected->left_id;
    }
    else
    {   // Идентификатор узла с минимальным ключом в правом поддереве.
        Node_t minimum_id = tree_minimum(tree, selected->right_id);
        // Узел с минимальным ключом в правом поддереве.
        TreeNode* minimum = tree_get(tree, minimum_id);
        // Правый дочерний узел для узла с минимальным ключом.
        Node_t t2_id = minimum->right_id;
        // s = selected_id
        // m = minimum_id
        /*   s     >
         *  / \    >   m
         * T1 ...  >  / \
         *    /    > T1 ...
         *   m     >    /
         *    \    >   T2
         *     T2  >
         */

        // Т.к. минимум ниже предыдущего узла, то балансировка будет идти с него.
        modified_node_was_black = minimum->is_black;
        rebalance_node_id = t2_id;

        if (minimum->parent_id == selected_id)
        {
            rebalance_parent_id = minimum_id;
        }
        else
        {   // Узел minimum не является правым дочерним узлом для узла selected.
            // Перевязываем поддерево T2 на место узла с минимальным ключом.
            tree_transplant(tree, minimum_id, t2_id);

            // Связываем узел minimum с правым дочерним узлом узла selected.
            minimum->right_id = selected->right_id;
            TreeNode* right = tree_get(tree, minimum->right_id);
            right->parent_id = minimum_id;
        }

        // Перевязываем узел minimum на место узла selected.
        tree_transplant(tree, selected_id, minimum_id);

        // Связываем узел minimum с левым дочерним узлом узла selected.
        minimum->left_id = selected->left_id;
        TreeNode* left = tree_get(tree, minimum->left_id);
        left->parent_id = minimum_id;

        // Устанавливаем цвет узла minimum равным цвету узла selected.
        minimum->is_black = selected->is_black;
    }

    tree_print(tree);
    printf("\n");
    sleep(1);

    if (modified_node_was_black)
    {   // Был удалён (перемещён и перекрашен) чёрный узел.
        tree_remove_fixup(tree, rebalance_node_id, rebalance_parent_id);
    }

    // Возвращаем значение из удаляемого узла.
    *ret = selected->value;

    tree_node_free(tree, selected_id);

    *found = true;
    return RET_OK;
}

//==================================================================================================
// Функция: tree_print
// Назначение: Производит печать красно-чёрного дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree (in) - красно-чёрное дерево.
//
// Возвращаемое значение:
// отсутствует.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
void tree_print(Tree* tree)
{
    int state[MAX_PRINT_DEPTH + 1U] = {};

    tree_print_recursive(tree, tree->root_id, 0U, state);
}


#endif // HEADER_GUARD_TREE_RB_H_INCLUDED