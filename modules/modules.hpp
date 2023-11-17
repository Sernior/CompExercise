/**
 * @file modules.cpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief modules implementation.
 * @version 0.1
 * @date 2023-11-17
 * @private
 * @copyright Copyright (c) 2023 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef MODULES_HPP
#define MODULES_HPP

#include<string>
#include<unordered_set>
#include<memory>

namespace modules{

    class basicBlock;

    class successor{

        public:

        successor(std::string const& tag, std::shared_ptr<basicBlock> basicBlockPtr) {
            if (tag != "Entry")
                mTag = tag;
            mInternalBasicBlockPtr = basicBlockPtr;
        }

        bool operator==(successor const& other) const {
            return mTag == other.mTag;
        }

        const std::string& getTag() const{
            return mTag;
        }

        basicBlock* operator->() const;

        basicBlock* operator*() const;

        struct Hash {
            size_t operator()(const successor& s) const {
                return std::hash<std::string>()(s.mTag);
            }
        };

        private:

        std::string mTag;
        std::shared_ptr<basicBlock> mInternalBasicBlockPtr;
    };

    class basicBlock{

        public:

        basicBlock(std::string const& name)
        : mName(name) {}

        basicBlock() = delete;

        const std::string& getName() const{
            return mName;
        }

        void addSuccessor(successor const& s){
            if (!mSuccessors.contains(s))
                mSuccessors.insert(s);
        }

        void removeSuccessor(successor const& s){
            if (mSuccessors.contains(s))
                mSuccessors.erase(s);
        }

        std::unordered_set<successor, successor::Hash>& getSuccessors(){
            return mSuccessors;
        }

        private:
        std::unordered_set<successor, successor::Hash> mSuccessors;
        std::string mName;

    };

    class function{

        public:

        function(std::string const& name)
        : mName(name)
        , mEntry("Entry") {}

        function() = delete;

        const std::string& getName() const{
            return mName;
        }

        bool operator==(function const& other) const {
            return mName == other.mName;
        }

        struct Hash {
            size_t operator()(const function& f) const {
                return std::hash<std::string>()(f.mName);
            }
        };

        basicBlock* getEntry(){
            return &mEntry;
        }

        private:
        
        std::string mName;
        basicBlock mEntry;

    };

    class module{

        public:

        module(std::string const& name)
        : mName(name)
        , mFunctions() {}

        module() = delete;

        const std::string& getName() const{
            return mName;
        }

        void addFunction(function const& f){
            mFunctions.emplace(f);
        }

        void removeFunction(function const& f){
            auto range = mFunctions.equal_range(f);
            mFunctions.erase(range.first, range.second);
        }

        private:

        std::string mName;
        std::unordered_multiset<function, function::Hash> mFunctions;

    };

}

modules::basicBlock* modules::successor::operator*() const {
    return mInternalBasicBlockPtr.get();
}

modules::basicBlock* modules::successor::operator->() const {
    return mInternalBasicBlockPtr.get();
}

#endif // MODULES_HPP
