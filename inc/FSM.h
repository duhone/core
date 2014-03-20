/*
 *  FSM.h
 *  BoB
 *
 *  Created by Eric Duhon on 1/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include "Property.h"

namespace CR
{
	namespace Core
	{		
		class IState
		{
		public:
			static const int UNCHANGED = -1;
			virtual bool Begin() {return false;}
			virtual void End() {}
			virtual int Process() {return UNCHANGED;}
		};
		
		class FSM
		{
		public:
			FSM() : m_state(-1), m_skipUpdate(false)
			{
				State.Init(this,&FSM::GetState,&FSM::SetState);
			}

			template<typename T, typename... Args>
			void Emplace(Args&&... args) {m_states.push_back(std::make_unique<T>(std::forward(args)...));}
			
			void operator++(int _postfix)
			{
				if(m_state < (m_states.size()-1))
					GoToState(m_state+1);
			}
			void operator--(int _postfix)
			{
				if(m_state > 0)
					GoToState(m_state-1);
			}
			void Reset() { m_state = -1;}
			void operator()()
			{
				if(m_state == -1)
					return;
				int new_state = IState::UNCHANGED;
				if(!m_skipUpdate && m_states[m_state] != NULL)
					new_state = m_states[m_state]->Process();
				m_skipUpdate = false;
				if(new_state != IState::UNCHANGED)
					GoToState(new_state);
			}			
			Property<int,FSM> State;
		private:
			bool m_skipUpdate; 
			int m_state;
			std::vector<std::unique_ptr<IState>> m_states;
			int GetState() const {return m_state;}
			void SetState(int _state) {GoToState(_state);}
			void GoToState(int _state)
			{			
				if(m_state >= 0)
					m_states[m_state]->End();
				m_state = _state;
				m_skipUpdate = m_states[m_state]->Begin();				
			}
		};
	}
}
