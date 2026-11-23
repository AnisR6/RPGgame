#pragma once

#include <functional>
#include <variant>
#include <vector>

template <typename Message> class MessageBus {
    public:
        template <typename T> void Subscribe(std::function<void(const T&)> callback) {
            subscribers.emplace_back([callback = std::move(callback)](const Message& message) {
                if (const auto* typedMessage = std::get_if<T>(&message)) {
                    callback(*typedMessage);
                }
            });
        }

        void Publish(const Message& message) const {
            for (const auto& subscriber : subscribers) {
                subscriber(message);
            }
        }

        void Clear() { subscribers.clear(); }

    private:
        std::vector<std::function<void(const Message&)>> subscribers;
};
