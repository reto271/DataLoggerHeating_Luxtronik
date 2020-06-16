#pragma once

/// The FeedbackCollector may be used to collect feedback of consecutive functions. The
///  collector might be initialized to true or false, depending on the outcome to be observed,
///  e.g. initialize it to true and use the AND feedback. If the feedback is still true after
///  all tests, we know that all feedback were true.
class FeedbackCollector
{
public:
    /// Constructor of the feedback collector.
    /// \param[in] initialState of the feedback collector
    FeedbackCollector(bool initialState);

    /// Adds a feedback. It does a logical AND with the current state of the
    /// \param[in] condition to be added
    void addAndFeedback(const bool condition);

    /// Adds a feedback. It does a logical OR with the current state of the
    /// \param[in] condition to be added
    void addOrFeedback(const bool condition);

    /// Returns the overall feedback
    /// \return overall feedback
    bool getFeedback();

private:
    bool m_currentState;   ///< Current state of the feedback collector.
};
