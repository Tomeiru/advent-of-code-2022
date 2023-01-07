#include <iostream>
#include <vector>
#include <list>
#include <numeric>

std::vector<std::string> get_file_content(const std::string &filepath);

typedef struct node_s {
    int64_t value;
    struct node_s *prev;
    struct node_s *next;
    struct node_s *next_id;
}node_t;

int64_t determine_new_index(const int64_t &index, const int64_t &value, const int64_t &size)
{
    int64_t new_index = index + value;

    if (new_index >= size)
        return (new_index % size);
    if (new_index < 0)
        return (size - (index % size));
    return new_index;
}

void swap_with_prev(node_t **node)
{
    node_t *prev = (*node)->prev;

    prev->prev->next = (*node);
    (*node)->prev = prev->prev;
    prev->prev = (*node);
    prev->next = (*node)->next;
    (*node)->next->prev = prev;
    (*node)->next = prev;
}

void swap_with_next(node_t **node)
{
    node_t *next = (*node)->next;

    next->next->prev = (*node);
    (*node)->prev->next = next;
    (*node)->next = next->next;
    next->prev = (*node)->prev;
    next->next = (*node);
    (*node)->prev = next;
}

int second(node_t *start, int64_t size) {
    node_t *temp = start;
    int64_t total = 0;

    for (uint64_t i = 0; i < 10; i++) {
        for (auto node = start; node != nullptr; node = node->next_id) {
            if (node->value == 0)
                continue;
            if (node->value < 0) {
                int64_t nb_swap = (node->value * -1) % (size - 1) * 811589153 % (size - 1);
                for (int64_t i = 0; i < nb_swap; i++)
                    swap_with_prev(&node);
                continue;
            }
            int64_t nb_swap = node->value % (size - 1) * 811589153 % (size - 1);
            for (int64_t i = 0; i < nb_swap; i++)
                swap_with_next(&node);
        }
    }
    while (temp->value != 0)
        temp = temp->next;
    temp = temp->next;
    for (uint64_t i = 0; i < 3000; i++) {
        if ((i + 1) % 1000 == 0) {
            total += temp->value * 811589153;
        }
        temp = temp->next;
    }
    std::cout << total << std::endl;
}

int first(node_t *start, int64_t size)
{
    node_t *temp = start;
    int64_t total = 0;

    for (auto node = start; node != nullptr; node = node->next_id) {
        if (node->value == 0)
            continue;
        if (node->value < 0) {
            int64_t nb_swap = (node->value * -1) % (size - 1);
            for (int64_t i = 0; i < nb_swap; i++)
                swap_with_prev(&node);
            continue;
        }
        int64_t nb_swap = node->value % (size - 1);
        for (int64_t i = 0; i < nb_swap; i++)
            swap_with_next(&node);
    }
    while (temp->value != 0)
        temp = temp->next;
    temp = temp->next;
    for (uint64_t i = 0; i < 3000; i++) {
        if ((i + 1) % 1000 == 0) {
            total += temp->value;
        }
        temp = temp->next;
    }
    std::cout << total << std::endl;
}

void append_to_end(node_t **start, node_t **end, int64_t value)
{
    if (*start == NULL && *end == NULL) {
        *start = new node_t;
        (*start)->value = value;
        (*start)->prev = nullptr;
        (*start)->next = nullptr;
        (*start)->next_id = nullptr;
        (*end) = (*start);
        return;
    }
    auto *elem = new node_t;
    elem->value = value;
    elem->prev = (*end);
    elem->next = nullptr;
    elem->next_id = nullptr;
    (*end)->next = elem;
    (*end)->next_id = elem;
    (*end) = elem;
}

int start(void)
{
    std::vector<std::string> file_content = get_file_content("../Day_20/input");
    node_t *start = nullptr;
    node_t *end = nullptr;
    int64_t size = 0;

    for (const auto &line: file_content) {
        append_to_end(&start, &end, std::stol(line));
        size++;
    }
    end->next = start;
    start->prev = end;
    //first(start, size);
    second(start, size);
    return (0);
}
