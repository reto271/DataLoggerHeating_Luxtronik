#include "FeedbackCollector.hpp"


FeedbackCollector::FeedbackCollector(bool initialState)
    : m_currentState(initialState)
{
}

void FeedbackCollector::addAndFeedback(const bool condition)
{
    m_currentState = m_currentState && condition;
}

void FeedbackCollector::addOrFeedback(const bool condition)
{
    m_currentState = m_currentState || condition;
}

bool FeedbackCollector::getFeedback()
{
    return m_currentState;
}
