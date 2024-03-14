#pragma once

#include "PopupNode.h"


struct FPopupStack
{
	FPopupStack(){};
    ~FPopupStack()
    {
        while (!IsEmpty()) {Pop();}
    }

	FPopupNode* Head{nullptr};

    void Push(UPopupWidget* Popup)
    {
        FPopupNode* NewNode = new FPopupNode();
        NewNode->Popup = Popup;

        if (IsEmpty())
        {
            Head = NewNode;
            return;
        }

        Head->Next = NewNode;
        Head = NewNode;
    }

    UPopupWidget* Pop()
    {
        if (IsEmpty()) {return nullptr;}

        UPopupWidget* Result = Head->Popup;
        FPopupNode* Next = Head->Next;

        delete Head;
        Head = Next;

        return Result;
    }

    UPopupWidget* Peak(int32 Idx)
    {
        if (IsEmpty()) {return nullptr;}

        FPopupNode* Node = Head;

        for (int32 i = 0; i < Idx; i++)
        {
            if (Node->Next == nullptr) {return nullptr;}
            Node = Node->Next;
        }

        return Node->Popup;
    }

    bool Remove(UPopupWidget* Popup)
    {
        if (IsEmpty() || Popup == nullptr) {return false;}

        FPopupNode* CurrentNode = Head;
        FPopupNode* PreviousNode = nullptr;
        do {
            if (CurrentNode->Popup == Popup)
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

    bool Contains(UPopupWidget* Popup)
    {
        if (Popup == nullptr) {return false;}

        FPopupNode* CurrentNode = Head;
        while (CurrentNode != nullptr && CurrentNode->Popup != Popup) {CurrentNode = CurrentNode->Next;}
        
        return CurrentNode->Popup == Popup;
    }

    bool IsEmpty()
    {
        return Head == nullptr;
    }

};
