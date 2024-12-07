// Copyright 2024 Vladislav Aleinik
#ifndef HEADER_GUARD_TREE_H_INCLUDED
#define HEADER_GUARD_TREE_H_INCLUDED

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

// Тип TreeNode - узел дерева
typedef struct {
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

    // Высота поддерева, имеющего данный узел как корневой.
    int32_t height;
} TreeNode;

// Тип Tree - двоичное дерево поиска.
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

//======================//
// Управление ресурсами //
//======================//

//==================================================================================================
// Функция: tree_alloc
// Назначение: Инициализирует бинарное дерево
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
    if (tree == NULL)
    {
        return RET_INVAL;
    }

    // Инициализируем поля структуры.
    tree->size     = 0U;
    tree->capacity = 1U;
    tree->root_id  = NULL_NODE;

    // Выделяем память для массива узлов дерева
    tree->nodes = calloc(tree->capacity, sizeof(TreeNode));
    if (tree->nodes == NULL)
    {
        return RET_NOMEM;
    }

    return RET_OK;
}

//==================================================================================================
// Функция: tree_free
// Назначение: Освобождает ресурсы дерева
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
    if (tree == NULL || tree->nodes == NULL)
    {
        return RET_INVAL;
    }

    // Освобождаем ранее выделенную динамическую память.
    free(tree->nodes);

    // Производим защиту от повторного освобождения памяти.
    tree->nodes = NULL;

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
// node_id (in) - валидный идентификатор узла дерева.
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
// Функция: tree_height
// Назначение: Возвращает высоту поддерева, имеющего заданный узел как корневой.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор корневого узла поддерева (валидный идентификатор или NULL_NODE).
//
// Возвращаемое значение:
// Высота поддерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
int32_t tree_height(Tree* tree, Node_t node_id)
{
    if (node_id == NULL_NODE)
    {
        return 0;
    }

    return tree->nodes[node_id].height;
}

//==================================================================================================
// Функция: tree_balance_factor
// Назначение: Возвращает разность высот левого и правого поддеревьев, заданных дочерними узлами
// заданного узла.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор узла (валидный идентификатор или NULL_NODE).
//
// Возвращаемое значение:
// Разность высот левого и правого поддеревьев, заданных дочерними узлами заданного узла.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
int32_t tree_balance_factor(Tree* tree, Node_t node_id)
{
    if (node_id == NULL_NODE)
    {
        return 0;
    }

    // Узел, соответствующий идентификтору node_id.
    TreeNode* node = tree_get(tree, node_id);

    return tree_height(tree, node->left_id) - tree_height(tree, node->right_id);
}

//==================================================================================================
// Функция: tree_transplant
// Назначение: Производит замену одного узла дерева на другой узел дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree            (in) - бинарное дерево поиска.
// transplanted_id (in) - идентификатор узла дерева, который будет заменён.
// new_id          (in) - идентификатор нового узла дерева, который заменит старый узел.
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
// - Балансировка дерева производится на более выооком уровне реализации.
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

        // Обновлояем идентификатор родительского узла для нового узла.
        new->parent_id = transplanted->parent_id;
    }
}

//==================================================================================================
// Функция: tree_node_allocate
// Назначение: Выделяет новый узел дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree     (in)  - бинарное дерево поиска.
// new_node (out) - идентификатора нового узла (выходной аргумент).
//
// Возвращаемое значение:
// Новый узел дерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
RetCode tree_node_allocate(Tree* tree, Node_t* new_node)
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
            return RET_NOMEM;
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

    // Высота нового узла равна 1.
    allocated->height = 1;

    // Возвращаем узел.
    *new_node = allocated_id;

    return RET_OK;
}

//==================================================================================================
// Функция: tree_node_free
// Назначение: Освбождает узел дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree     (in) - бинарное дерево поиска.
// freed_id (in) - идентификатора узла для освобождения.
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
// tree       (in)  - бинарное дерево поиска.
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
// tree       (in) - бинарное дерево поиска.
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
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор корневого узла поддерева (валидный идентификатор или NULL_NODE).
//
// Возвращаемое значение:
// Идентификатор нового корневого узла поддерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Возможно визуализация преобразования, заданного данной функцией.
//   Здесь узел node задан идентификатором node_id, а узел ret - ret_id.
/*    node                     ret
 *    / \                      / \
 *   T1 ret     ------->     node T3
 *      / \                  / \
 *     T2  T3               T1  T2
 */
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

    // Обновляем высоты обоих узлов.
    node->height = max(tree_height(tree, node->left_id), tree_height(tree, node->right_id)) + 1;
    ret->height  = max(node->height, tree_height(tree, ret->right_id)) + 1;

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
// tree    (in) - бинарное дерево поиска.
// node_id (in) - идентификатор корневого узла поддерева (валидный идентификатор или NULL_NODE).
//
// Возвращаемое значение:
// Идентификатор нового корневого узла поддерева.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Возможно визуализация преобразования, заданного данной функцией.
//   Здесь узел node задан идентификатором node_id, а узел ret - ret_id.
/*      node                ret
 *      / \                 / \
 *    ret  T3   ------->   T1 node
 *    / \                     / \
 *   T1  T2                  T2  T3
 */
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

    // Обновляем высоты обоих узлов.
    node->height = max(tree_height(tree, node->left_id), tree_height(tree, node->right_id)) + 1;
    ret->height  = max(node->height, tree_height(tree, ret->left_id)) + 1;

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

//==================================================================================================
// Функция: tree_print_recursive
// Назначение: Рекурсивно производит печать поддерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree   (in) - бинарное дерево поиска.
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
        printf("[%d] (%d)\n", node->key, node->height);
    }

    // Рекурсивно печатаем правое поддерево.
    state[level] = 2;
    if (node != NULL)
    {
        tree_print_recursive(tree, node->left_id, level + 1U, state);
    }
    state[level] = 3;
}

// Предварительная декларация функции для печати.
void tree_print(Tree* tree);

//==================================================================================================
// Функция: tree_balance
// Назначение: Производит перебалансировку дерева от нижнего узла к верхнему.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree        (in) - бинарное дерево поиска.
// unbalanced_id (in) - идентификатор узла, с которого требуется начинать перебалансировку.
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
void tree_balance(Tree* tree, Node_t unbalanced_id)
{
    if (unbalanced_id == NULL_NODE)
    {
        // Для узла-пустышки балансировка уже выполнена.
        return;
    }

    // Производим итеративную балансировку дерева.
    do
    {
        // Узел, являющийся текущим кандидатом на перебалансировку.
        TreeNode* unbalanced = tree_get(tree, unbalanced_id);

        // Родительский узел для итогового поддерева.
        Node_t parent_id = unbalanced->parent_id;

        // Вычисляем разность высот левого и правого поддеревьев.
        int32_t balance_factor = tree_balance_factor(tree, unbalanced_id);

        // Проверяем необходимость балансировки.
        if (balance_factor > 1)
        {   // Левое поддерево значительно выше правого.
            // Идентификатор корня левого поддерева.
            Node_t left_id = unbalanced->left_id;

            // Проверяем баланс левого поддерева.
            if (tree_balance_factor(tree, left_id) < 0)
            {
                // u = unbalanced_id
                // l = left_id
                /*     u     >       u
                 *    / \    >      / \
                 *   l   T4  >     x   T4
                 *  / \      >    / \
                 * T1  x     >   l  T3
                 *    / \    >  / \
                 *   T2 T3   > T1 T2
                 */

                // Производим дополнительный обратный поворот.
                tree_rotate_left(tree, left_id);
            }

            // u = unbalanced_id
            // l = left_id
            /*       u    >      l
             *      / \   >     / \
             *     l   T4 >    /   \
             *    / \     >   x     u
             *   x  T3    >  / \   / \
             *  / \       > T1 T2 T3 T4
             * T1 T2      >
             */

            // Производим прямой поворот.
            tree_rotate_right(tree, unbalanced_id);
        }
        else if (balance_factor < -1)
        {   // Правое поддерево значительно выше левого.
            // Идентификатор корня правого поддерева.
            Node_t right_id = unbalanced->right_id;

            // Проверяем баланс правого поддерева.
            if (tree_balance_factor(tree, right_id) > 0)
            {
                // u = unbalanced_id
                // r = right_id
                /*    u     >   u
                 *   / \    >  / \
                 *  T1  r   > T1  r
                 *     / \  >    / \
                 *    x  T4 >   T2  x
                 *   / \    >      / \
                 *  T2 T3   >     T3 T4
                 */

                // Производим дополнительный обратный поворот.
                tree_rotate_right(tree, right_id);
            }

            // u = unbalanced_id
            // r = right_id
            /*   u       >      r
             *  / \      >     / \
             * T1  r     >    /   \
             *    / \    >   u     x
             *   T2  x   >  / \   / \
             *      / \  > T1 T2 T3 T4
             *     T3 T4 >
             */

            // Производим прямой поворот.
            tree_rotate_left(tree, unbalanced_id);
        }
        else
        {
            // Вычисляем высоту текущего кандидата на перебалансировку.
            unbalanced->height = max(tree_height(tree, unbalanced->left_id), tree_height(tree, unbalanced->right_id)) + 1;
        }

        tree_print(tree);
        printf("\n");
        sleep(1);

        // Переходим к рассмотрению родительского узла.
        unbalanced_id = parent_id;
    }
    while (unbalanced_id != NULL_NODE);
}


//============================//
// Пользовательский интерфейс //
//============================//

//==================================================================================================
// Функция: tree_search
// Назначение: Находит значение в дереве по ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree  (in)  - бинарное дерево поиска.
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
// tree  (in) - бинарное дерево поиска.
// key   (in) - ключ, по которому производится поиск значения.
// value (in) - новое значение для заданного ключа.
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
    Node_t allocated_id;
    RetCode ret = tree_node_allocate(tree, &allocated_id);
    if (ret != RET_OK)
    {
        // Структура дерева не изменилась, перебалансировка не требуется.
        return ret;
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
        else if (parent->key < key)
        {
            parent->right_id = allocated_id;
        }
    }

    // Инициализируем выделенный узел.
    TreeNode* allocated = tree_get(tree, allocated_id);

    allocated->parent_id = parent_id;
    allocated->key       = key;
    allocated->value     = value;

    // Производим балансировку дерева, начиная с добавленного узла.
    tree_balance(tree, allocated_id);

    return RET_OK;
}

//==================================================================================================
// Функция: tree_remove
// Назначение: Удаляет ключ из дерева с возвратом значения.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree  (in)  - бинарное дерево поиска.
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
    if (tree == NULL || ret == NULL || found == NULL)
    {
        return RET_INVAL;
    }

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

    if (selected->left_id == NULL_NODE)
    {   // У найденного узла отсутствует левый дочерный узел.
        tree_transplant(tree, selected_id, selected->right_id);

        // Выполняем балансировку, начиная с изменённого поддерева.
        tree_balance(tree, selected_parent_id);
    }
    else if (selected->right_id == NULL_NODE)
    {
        // У найденного узла отсутствует правый дочерный узел.
        tree_transplant(tree, selected_id, selected->left_id);

        // Выполняем балансировку, начиная с изменённого поддерева.
        tree_balance(tree, selected_parent_id);
    }
    else
    {   // У найденного узла два дочерних узла.
        // Идентификатор узла с минимальным ключом в правом поддереве.
        Node_t minimum_id = tree_minimum(tree, selected->right_id);

        // Узел с минимальным ключом в правом поддереве.
        TreeNode* minimum = tree_get(tree, minimum_id);

        // Родительский узел для узла с минимальным ключом.
        Node_t minimum_parent_id = minimum->parent_id;

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

        if (minimum_parent_id != selected_id)
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

        // Производим перебалансировку с низшего застронутого узла.
        if (minimum_parent_id != selected_id)
        {   // Низшим затронутым узлом является узел с идентификатором minimum_parent_id.
            tree_balance(tree, minimum_parent_id);
        }
        else
        {   // Низшим затронутым узлом является узел minimum.
            tree_balance(tree, minimum_id);
        }
    }

    // Возвращаем значение из удаляемого узла.
    *ret = selected->value;

    tree_node_free(tree, selected_id);

    *found = true;
    return RET_OK;
}

//==================================================================================================
// Функция: tree_print
// Назначение: Производит печать дерева.
//--------------------------------------------------------------------------------------------------
// Параметры:
// tree (in) - бинарное дерево поиска.
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

    Node_t root_id = tree->root_id;
    tree_print_recursive(tree, root_id, 0U, state);
}

#endif // HEADER_GUARD_TREE_H_INCLUDED
