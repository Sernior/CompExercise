/**
 * @file test.cpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief test.
 * @version 0.1
 * @date 2023-11-17
 * @private
 * @copyright Copyright (c) 2023 Federico Abrignani (federignoli@hotmail.it).
 * 
 */
#include <gtest/gtest.h>
#include <modules.hpp>
#include <vector>

namespace DoubleEntryFunction{
    modules::function getFunction(){
        auto func = modules::function("function name");
        auto entry = func.getEntry();
        entry->addSuccessor(modules::successor("1", std::make_shared<modules::basicBlock>("Entry")));
        return func;
    }
}

namespace SingleEntryFunction{
    modules::function getFunction(){
        auto func = modules::function("function name");
        auto entry = func.getEntry();
        entry->addSuccessor(modules::successor("1", std::make_shared<modules::basicBlock>("NotAnEntry")));
        return func;
    }
}

namespace FiveBasicBlocks{
    static int expected = 5;
    static int expectedComplex = 13;

    modules::function getFunction(){
        auto func = modules::function("function name");
        auto entry = func.getEntry();
        for (int i = 1; i < expected; ++i)
            entry->addSuccessor(modules::successor(std::to_string(i), std::make_shared<modules::basicBlock>(std::to_string(i))));
        return func;
    }

    modules::function getFunctionComplex(){ // entry + 4 entry successors + 2*4 (each entry succ has 2 succ) = 13;
        auto func = modules::function("function name");
        auto entry = func.getEntry();
        for (int i = 1; i < expected; ++i){
            auto bbPtr = std::make_shared<modules::basicBlock>(std::to_string(i));
            entry->addSuccessor(modules::successor(std::to_string(i), bbPtr));
            for (int j = 0; j < 2; ++j)
                bbPtr->addSuccessor(modules::successor(std::to_string(i).append(std::to_string(j)), std::make_shared<modules::basicBlock>(std::to_string(i).append(std::to_string(j)))));
        }
        return func;
    }

}

TEST(OneAndOnlyOneEntry, DoubleEntry) {
    auto func = DoubleEntryFunction::getFunction();
    std::unordered_set<modules::basicBlock*> visited;
    std::vector<modules::basicBlock*> stack;
    stack.push_back(func.getEntry());
    bool hasEntry{};
    while (!stack.empty()){
        auto bb = stack.back();
        stack.pop_back();
        visited.insert(bb);
        if (hasEntry && bb->getName() == "Entry")
            ASSERT_TRUE(false);
        if (bb->getName() == "Entry")
            hasEntry = true;
        for (auto& ss : bb->getSuccessors()){
            if (visited.contains(*ss)) continue;
            stack.push_back(*ss);
        }
    }
    ASSERT_TRUE(true);
}

TEST(OneAndOnlyOneEntry, Reachable) {
    auto func = SingleEntryFunction::getFunction();
    std::unordered_set<modules::basicBlock*> visited;
    std::vector<modules::basicBlock*> stack;
    stack.push_back(func.getEntry());
    bool hasEntry{};
    while (!stack.empty()){
        auto bb = stack.back();
        stack.pop_back();
        visited.insert(bb);
        if (hasEntry && bb->getName() == "Entry")
            ASSERT_TRUE(false);
        if (bb->getName() == "Entry")
            hasEntry = true;
        for (auto& ss : bb->getSuccessors()){
            if (visited.contains(*ss)) continue;
            stack.push_back(*ss);
        }
    }
    ASSERT_TRUE(true);
}

TEST(EachBasicBlockReachable, FiveBlocks) {
    auto func = FiveBasicBlocks::getFunction();
    std::unordered_set<modules::basicBlock*> visited;
    std::vector<modules::basicBlock*> stack;
    stack.push_back(func.getEntry());
    int count = 0;
    while (!stack.empty()){
        auto bb = stack.back();
        stack.pop_back();
        visited.insert(bb);
        ++count;
        for (auto& ss : bb->getSuccessors()){
            if (visited.contains(*ss)) continue;
            stack.push_back(*ss);
        }
    }
    ASSERT_TRUE(FiveBasicBlocks::expected == count);
}

TEST(EachBasicBlockReachable, ThirteenBlocks) {
    auto func = FiveBasicBlocks::getFunctionComplex();
    std::unordered_set<modules::basicBlock*> visited;
    std::vector<modules::basicBlock*> stack;
    stack.push_back(func.getEntry());
    int count = 0;
    while (!stack.empty()){
        auto bb = stack.back();
        stack.pop_back();
        visited.insert(bb);
        ++count;
        for (auto& ss : bb->getSuccessors()){
            if (visited.contains(*ss)) continue;
            stack.push_back(*ss);
        }
    }
    ASSERT_TRUE(FiveBasicBlocks::expectedComplex == count);
}

TEST(OnlyOneTag, Positive) {
    auto func = FiveBasicBlocks::getFunctionComplex();
    std::vector<modules::basicBlock*> stack;
    std::unordered_set<modules::basicBlock*> visited;
    stack.push_back(func.getEntry());
    std::unordered_set<std::string> tagsInAdj;
    while (!stack.empty()){
        auto bb = stack.back();
        stack.pop_back();
        visited.insert(bb);
        for (auto& ss : bb->getSuccessors()){
            if (tagsInAdj.contains(ss.getTag()))
                ASSERT_TRUE(false);
            tagsInAdj.insert(ss.getTag());
            if (visited.contains(*ss)) continue;
            stack.push_back(*ss);
        }
        tagsInAdj.clear();
    }
    ASSERT_TRUE(true);
}

/*

"Write a serialization routine to GraphViz of the control flow graph:"

I do not quite get the meaning of this question.

In GraphViz the graph you provided is writted as:

digraph ExampleGraph {
    Entry -> H [label="true"];
    Entry -> I [label="false"];
    H -> J;
    I -> J;
}

I guess this is what you are asking for? Or you want me to make that graph with my module class?

*/

int main(int argc, char* argv[]) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
