#include "FeedbackCollector.hpp"


FeedbackCollector::FeedbackCollector(bool initialState)
    : m_currentState(initialState)
{
}

FeedbackCollector::~FeedbackCollector()
{
}

void FeedbackCollector::addAndFeedback(const bool condition)
{
    m_currentState = m_currentState && condition;
}

void FeedbackCollector::addOrFeeback(const bool condition)
{
    m_currentState = m_currentState || condition;
}

bool FeedbackCollector::getFeedback()
{
    return m_currentState;
}
