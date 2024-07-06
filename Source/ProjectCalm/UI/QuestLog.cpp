#include "QuestLog.h"
#include "ProjectCalm/Game/Quests/QuestDetails.h"
#include "ProjectCalm/UI/TextTreeNodeWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UQuestLog::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogGameMode, "UQuestLog:: Failed to get game mode!");

    TArray<FQuestDetails> ActiveQuests;
    GameMode->GetActiveQuests(ActiveQuests);
    for (FQuestDetails Quest : ActiveQuests) {CreateQuestEntry(Quest);}
}

void UQuestLog::CreateQuestEntry(FQuestDetails Quest)
{
    if (TaskList == nullptr) {return;}

    UTextTreeNodeWidget* QuestTitleWidget = CreateWidget<UTextTreeNodeWidget>(this, RootWidgetClass);
    if (QuestTitleWidget == nullptr) {return;}

    QuestTitleWidget->SetText(Quest.Name.ToString());
    TaskList->AddChildToVerticalBox(QuestTitleWidget);

    UTextTreeNodeWidget* QuestDescWidget = CreateWidget<UTextTreeNodeWidget>(this, LeafWidgetClass);
    if (QuestDescWidget != nullptr)
    {
        QuestDescWidget->SetText(Quest.QuestDescription);
        TaskList->AddChildToVerticalBox(QuestDescWidget);
        QuestTitleWidget->AddChild(QuestDescWidget);
    }

    for (int32 i = 0; i < Quest.ActiveStageIdx + 1; i++) {CreateStageEntry(Quest.Stages[i], QuestTitleWidget);}

    QuestTitleWidget->Setup();
}

void UQuestLog::CreateStageEntry(FStageDetails Stage, UTextTreeNodeWidget* Parent)
{
    if (TaskList == nullptr || Parent == nullptr) {return;}

    UTextTreeNodeWidget* StageWidget = CreateWidget<UTextTreeNodeWidget>(this, BranchWidgetClass);
    if (StageWidget == nullptr) {return;}

    StageWidget->SetText(Stage.StageDescription);
    if (Stage.bIsComplete)
    {
        StageWidget->SetTextColor(CompletedColor);
        StageWidget->ToggleStrikethrough();
    }
    UVerticalBoxSlot* ChildSlot = TaskList->AddChildToVerticalBox(StageWidget);
    if (ChildSlot != nullptr) {ChildSlot->SetPadding(FMargin(Indent, 0, 0, 0));}
    Parent->AddChild(StageWidget);

    for (FObjectiveDetails Objective : Stage.Objectives) {CreateObjectiveEntry(Objective, StageWidget);}
}

void UQuestLog::CreateObjectiveEntry(FObjectiveDetails Objective, UTextTreeNodeWidget* Parent)
{
    if (TaskList == nullptr || Parent == nullptr) {return;}

    UTextTreeNodeWidget* ObjectiveWidget = CreateWidget<UTextTreeNodeWidget>(this, LeafWidgetClass);
    if (ObjectiveWidget == nullptr) {return;}

    ObjectiveWidget->SetText(Objective.ObjectiveDescription);
    if (Objective.bIsComplete)
    {
        ObjectiveWidget->SetTextColor(CompletedColor);
        ObjectiveWidget->ToggleStrikethrough();
    }
    UVerticalBoxSlot* ChildSlot = TaskList->AddChildToVerticalBox(ObjectiveWidget);
    if (ChildSlot != nullptr) {ChildSlot->SetPadding(FMargin(Indent, 0, 0, 0));}
    Parent->AddChild(ObjectiveWidget);
}
