#pragma once
#include "Define.h"

class CBehaviorTree abstract
{
public:
	enum State { Failure, Success, Running};

public:
	CBehaviorTree() {}
	virtual ~CBehaviorTree() {}

public:
	virtual State Run()		PURE;
};

// 전이 담당 노드
class CBT_Composite abstract : public CBehaviorTree
{
public:
	CBT_Composite() {}
	virtual ~CBT_Composite() {}

public:
	virtual State Run() override		PURE;

protected:
	list<CBehaviorTree*>	m_Children;
};



class CBT_Sequence abstract : public CBT_Composite
{
public:
	CBT_Sequence() {}
	virtual ~CBT_Sequence() {}

public:
	virtual State Run() override
	{
		State eState = State::Success;

		for (auto& node : m_Children)
		{
			State eReturnState = node->Run();
			if (State::Failure == eReturnState)
			{
				return eState = State::Failure;
			}
			else if (State::Running == eReturnState)
			{
				return eState = State::Running;
			}
		}

		return eState;
	}
};

class CBT_Selector abstract : public CBT_Composite
{
public:
	CBT_Selector() {}
	virtual ~CBT_Selector() {}

public:
	virtual State Run() override
	{
		for (auto& node : m_Children)
		{
			State eState = node->Run();
			if (State::Running == eState)
			{
				return State::Running;
			}
			else if (State::Failure == eState)
			{
				return State::Failure;
			}
			
		}

		return State::Success;
	}
};

class CBT_Parallel abstract : public CBT_Composite
{
public:
	CBT_Parallel() {}
	virtual ~CBT_Parallel() {}

public:
	virtual State Run() override
	{
		for (auto& node : m_Children)
		{
			State eState = node->Run();
			if (State::Running == eState)
			{
				return State::Running;
			}
			else if (State::Success == eState)
			{
				return State::Success;
			}
			
		}

		return State::Failure;
	}
};

// 조건 노드
class CBT_Decorator abstract : public CBehaviorTree
{
public:
	CBT_Decorator() {}
	virtual ~CBT_Decorator() {}

public:
	virtual State Run() override		PURE;

protected:
	CBehaviorTree* m_pChild;
};

class CBT_Condition abstract : public CBT_Decorator
{
public:
	CBT_Condition() {}
	virtual ~CBT_Condition() {}

public:
	virtual State Run() override
	{
		return m_pChild->Run();
	}
};

class CBT_Inverter abstract : public CBT_Decorator
{
public:
	CBT_Inverter() {}
	virtual ~CBT_Inverter() {}

public:
	virtual State Run() override
	{
		State eState = m_pChild->Run();
		return (State::Running  == eState) ? State::Running : eState;
	}
};

// 끝 노드(액션)
class CBT_Task abstract : public CBehaviorTree
{
public:
	CBT_Task() {}
	virtual ~CBT_Task() {}

public:
	virtual State Run()		PURE;
};

