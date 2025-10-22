#include "TError.h"
#include "TString.h"
#include "TQueue.h"
#include "TStack.h"

#include <gtest.h>

TEST(TQueue, can_create_queue_with_positive_capacity)
{
    ASSERT_NO_THROW(TQueue<int> queue(5));
    ASSERT_NO_THROW(TQueue<int> queue(1));
}

TEST(TQueue, can_create_default_queue)
{
    ASSERT_NO_THROW(TQueue<int> queue);
}

TEST(TQueue, can_copy_queue)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);

    TQueue<int> queueCopy(queue);
    EXPECT_TRUE(queue == queueCopy);
}

TEST(TQueue, can_move_queue)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);

    TQueue<int> queueMoved(std::move(queue));
    EXPECT_EQ(1, queueMoved.Head());
    EXPECT_EQ(2, queueMoved.Tail());
    EXPECT_TRUE(queue.IsEmpty());
}

TEST(TQueue, can_get_size)
{
    TQueue<int> queue(5);
    EXPECT_EQ(0, queue.Size());

    queue.Put(1);
    EXPECT_EQ(1, queue.Size());
}

TEST(TQueue, can_check_empty)
{
    TQueue<int> emptyQueue;
    TQueue<int> nonEmptyQueue(2);
    nonEmptyQueue.Put(1);

    EXPECT_TRUE(emptyQueue.IsEmpty());
    EXPECT_FALSE(nonEmptyQueue.IsEmpty());
}

TEST(TQueue, can_check_full)
{
    TQueue<int> queue(2);
    queue.Put(1);
    queue.Put(2);

    EXPECT_TRUE(queue.IsFull());
}

TEST(TQueue, can_put_element)
{
    TQueue<int> queue(3);

    ASSERT_NO_THROW(queue.Put(1));
    ASSERT_NO_THROW(queue.Put(2));
    ASSERT_NO_THROW(queue.Put(3));

    EXPECT_EQ(3, queue.Size());
}

TEST(TQueue, throw_put_when_full)
{
    TQueue<int> queue(2);
    queue.Put(1);
    queue.Put(2);

    ASSERT_ANY_THROW(queue.Put(3));
}

TEST(TQueue, can_get_element)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);

    EXPECT_EQ(1, queue.Get());
    EXPECT_EQ(2, queue.Get());
    EXPECT_EQ(3, queue.Get());
}

TEST(TQueue, throw_get_when_empty)
{
    TQueue<int> queue;

    ASSERT_ANY_THROW(queue.Get());
}

TEST(TQueue, can_get_head)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);

    EXPECT_EQ(1, queue.Head());
}

TEST(TQueue, throw_head_when_empty)
{
    TQueue<int> queue;

    ASSERT_ANY_THROW(queue.Head());
}

TEST(TQueue, can_get_tail)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);

    EXPECT_EQ(2, queue.Tail());
}

TEST(TQueue, throw_tail_when_empty)
{
    TQueue<int> queue;

    ASSERT_ANY_THROW(queue.Tail());
}

TEST(TQueue, can_find_min)
{
    TQueue<int> queue(5);
    queue.Put(5);
    queue.Put(2);
    queue.Put(8);
    queue.Put(1);
    queue.Put(4);

    EXPECT_EQ(1, queue.Min());
}

TEST(TQueue, throw_min_when_empty)
{
    TQueue<int> queue;

    ASSERT_ANY_THROW(queue.Min());
}

TEST(TQueue, can_access_by_index)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);

    EXPECT_EQ(1, queue[0]);
    EXPECT_EQ(2, queue[1]);
    EXPECT_EQ(3, queue[2]);
}

TEST(TQueue, throw_access_by_invalid_index)
{
    TQueue<int> queue(2);
    queue.Put(1);

    ASSERT_ANY_THROW(queue[2]);
    ASSERT_ANY_THROW(queue[5]);
}

TEST(TQueue, can_check_equality)
{
    TQueue<int> queue1(3);
    queue1.Put(1);
    queue1.Put(2);

    TQueue<int> queue2(3);
    queue2.Put(1);
    queue2.Put(2);

    EXPECT_TRUE(queue1 == queue2);
}

TEST(TQueue, can_check_inequality)
{
    TQueue<int> queue1(3);
    queue1.Put(1);
    queue1.Put(2);

    TQueue<int> queue2(3);
    queue2.Put(1);
    queue2.Put(3);

    EXPECT_TRUE(queue1 != queue2);
}

TEST(TQueue, can_assignment)
{
    TQueue<int> queue1(3);
    queue1.Put(1);
    queue1.Put(2);

    TQueue<int> queue2;
    queue2 = queue1;

    EXPECT_TRUE(queue1 == queue2);
}

TEST(TQueue, can_move_assignment)
{
    TQueue<int> queue1(3);
    queue1.Put(1);
    queue1.Put(2);

    TQueue<int> queue2;
    queue2 = std::move(queue1);

    EXPECT_EQ(1, queue2.Head());
    EXPECT_EQ(2, queue2.Tail());
    EXPECT_TRUE(queue1.IsEmpty());
}

TEST(TQueue, can_use_iterators)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);

    int sum = 0;
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(TQueue, can_use_const_iterators)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);

    int sum = 0;
    for (auto it = queue.cbegin(); it != queue.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(TQueue, can_save_to_file)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);

    ASSERT_NO_THROW(queue.WriteToFile("test_queue.txt"));
}

TEST(TQueue, can_load_from_file)
{
    TQueue<int> queue(3);
    queue.Put(1);
    queue.Put(2);
    queue.Put(3);
    queue.WriteToFile("test_queue.txt");

    TQueue<int> loadedQueue;
    ASSERT_NO_THROW(loadedQueue.ReadFromFile("test_queue.txt"));

    EXPECT_TRUE(queue == loadedQueue);
}

TEST(TStack, can_create_stack_with_positive_capacity)
{
    ASSERT_NO_THROW(TStack<int> stack(5));
    ASSERT_NO_THROW(TStack<int> stack(1));
}

TEST(TStack, can_create_default_stack)
{
    ASSERT_NO_THROW(TStack<int> stack);
}

TEST(TStack, can_copy_stack)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);

    TStack<int> stackCopy(stack);
    EXPECT_TRUE(stack == stackCopy);
}

TEST(TStack, can_move_stack)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);

    TStack<int> stackMoved(std::move(stack));
    EXPECT_EQ(2, stackMoved.Top());
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStack, can_get_size)
{
    TStack<int> stack(5);
    EXPECT_EQ(0, stack.Size());

    stack.Put(1);
    EXPECT_EQ(1, stack.Size());
}

TEST(TStack, can_check_empty)
{
    TStack<int> emptyStack;
    TStack<int> nonEmptyStack(2);
    nonEmptyStack.Put(1);

    EXPECT_TRUE(emptyStack.IsEmpty());
    EXPECT_FALSE(nonEmptyStack.IsEmpty());
}

TEST(TStack, can_check_full)
{
    TStack<int> stack(2);
    stack.Put(1);
    stack.Put(2);

    EXPECT_TRUE(stack.IsFull());
}

TEST(TStack, can_put_element)
{
    TStack<int> stack(3);

    ASSERT_NO_THROW(stack.Put(1));
    ASSERT_NO_THROW(stack.Put(2));
    ASSERT_NO_THROW(stack.Put(3));

    EXPECT_EQ(3, stack.Size());
}

TEST(TStack, throw_put_when_full)
{
    TStack<int> stack(2);
    stack.Put(1);
    stack.Put(2);

    ASSERT_ANY_THROW(stack.Put(3));
}

TEST(TStack, can_get_element)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);

    EXPECT_EQ(3, stack.Get());
    EXPECT_EQ(2, stack.Get());
    EXPECT_EQ(1, stack.Get());
}

TEST(TStack, throw_get_when_empty)
{
    TStack<int> stack;

    ASSERT_ANY_THROW(stack.Get());
}

TEST(TStack, can_get_top)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);

    EXPECT_EQ(2, stack.Top());
}

TEST(TStack, throw_top_when_empty)
{
    TStack<int> stack;

    ASSERT_ANY_THROW(stack.Top());
}

TEST(TStack, can_find_min)
{
    TStack<int> stack(5);
    stack.Put(5);
    stack.Put(2);
    stack.Put(8);
    stack.Put(1);
    stack.Put(4);

    EXPECT_EQ(1, stack.Min());
}

TEST(TStack, throw_min_when_empty)
{
    TStack<int> stack;

    ASSERT_ANY_THROW(stack.Min());
}

TEST(TStack, can_access_by_index)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);

    EXPECT_EQ(1, stack[0]);
    EXPECT_EQ(2, stack[1]);
    EXPECT_EQ(3, stack[2]);
}

TEST(TStack, throw_access_by_invalid_index)
{
    TStack<int> stack(2);
    stack.Put(1);

    ASSERT_ANY_THROW(stack[2]);
    ASSERT_ANY_THROW(stack[5]);
}

TEST(TStack, can_check_equality)
{
    TStack<int> stack1(3);
    stack1.Put(1);
    stack1.Put(2);

    TStack<int> stack2(3);
    stack2.Put(1);
    stack2.Put(2);

    EXPECT_TRUE(stack1 == stack2);
}

TEST(TStack, can_check_inequality)
{
    TStack<int> stack1(3);
    stack1.Put(1);
    stack1.Put(2);

    TStack<int> stack2(3);
    stack2.Put(1);
    stack2.Put(3);

    EXPECT_TRUE(stack1 != stack2);
}

TEST(TStack, can_assignment)
{
    TStack<int> stack1(3);
    stack1.Put(1);
    stack1.Put(2);

    TStack<int> stack2;
    stack2 = stack1;

    EXPECT_TRUE(stack1 == stack2);
}

TEST(TStack, can_move_assignment)
{
    TStack<int> stack1(3);
    stack1.Put(1);
    stack1.Put(2);

    TStack<int> stack2;
    stack2 = std::move(stack1);

    EXPECT_EQ(2, stack2.Top());
    EXPECT_TRUE(stack1.IsEmpty());
}

TEST(TStack, can_use_iterators)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);

    int sum = 0;
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(TStack, can_use_const_iterators)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);

    int sum = 0;
    for (auto it = stack.cbegin(); it != stack.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(6, sum);
}

TEST(TStack, can_save_to_file)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);

    ASSERT_NO_THROW(stack.WriteToFile("test_stack.txt"));
}

TEST(TStack, can_load_from_file)
{
    TStack<int> stack(3);
    stack.Put(1);
    stack.Put(2);
    stack.Put(3);
    stack.WriteToFile("test_stack.txt");

    TStack<int> loadedStack;
    ASSERT_NO_THROW(loadedStack.ReadFromFile("test_stack.txt"));

    EXPECT_TRUE(stack == loadedStack);
}