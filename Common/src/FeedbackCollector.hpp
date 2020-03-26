#pragma once


class FeedbackCollector
{
public:
    FeedbackCollector(bool initialState);
    virtual ~FeedbackCollector();

    void addAndFeedback(const bool condition);
    void addOrFeedback(const bool condition);

    bool getFeedback();

private:
    bool m_currentState;
};
