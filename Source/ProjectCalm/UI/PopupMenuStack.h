#pragma once

#include "PopupMenuNode.h"


struct FPopupMenuStack
{
	FPopupMenuStack(){};
    ~FPopupMenuStack()
    {
        while (!IsEmpty()) {Pop();}
    }

	FPopupMenuNode* Head{nullptr};

    void Push(UPopupMenu* PopupMenu)
    {
        FPopupMenuNode* NewNode = new FPopupMenuNode();
        NewNode->Menu = PopupMenu;

        if (IsEmpty())
        {
            Head = NewNode;
            return;
        }

        Head->Next = NewNode;
        Head = NewNode;
    }

    UPopupMenu* Pop()
    {
        if (IsEmpty()) {return nullptr;}

        UPopupMenu* Result = Head->Menu;
        FPopupMenuNode* Next = Head->Next;

        delete Head;
        Head = Next;

        return Result;
    }

    bool Remove(UPopupMenu* PopupMenu)
    {
        if (IsEmpty() || PopupMenu == nullptr) {return false;}

        FPopupMenuNode* CurrentNode = Head;
        FPopupMenuNode* PreviousNode = nullptr;
        do {
            if (CurrentNode->Menu == PopupMenu)
            {
                if (CurrentNode == Head) {Head = CurrentNode->Next;}
                else {PreviousNode->Next = CurrentNode->Next;}
                
                delete CurrentNode;
                return true;
            }

            PreviousNode = CurrentNode;
            CurrentNode = CurrentNode->Next;
        } while (CurrentNode != nullptr);

        return false;
    }

    bool Contains(UPopupMenu* PopupMenu)
    {
        if (PopupMenu == nullptr) {return false;}

        FPopupMenuNode* CurrentNode = Head;
        while (CurrentNode != nullptr && CurrentNode->Menu != PopupMenu) {CurrentNode = CurrentNode->Next;}
        
        return CurrentNode->Menu == PopupMenu;
    }

    bool IsEmpty()
    {
        return Head == nullptr;
    }

};
