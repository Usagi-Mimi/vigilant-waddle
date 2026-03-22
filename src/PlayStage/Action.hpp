#ifndef ACTION_HPP
#define ACTION_HPP

#include <variant>

namespace vw
{

struct Move
{
    int x;
    int y;
};

struct Wait
{
    // No data for this one; this is basically "passing" on a turn.
};

/**
 * @brief An action/command that entities (like the player) can take
 */
using Action = std::variant<Move,
                            Wait>;

/**
 * @brief Helps code react to actions in a more streamlined way than with
 * conditionals on `std::holds_alternative()`
 *
 * This lets you define a set of overloads for an action-bearing `std::variant`.
 * You can then pass that set to `std::visit()` to "react" to that variant's
 * underlying/relevant actions. For example:
 * ```
 * Action a; // `std::variant<Move, Attack, ...>`
 * auto const r = Reactions
 * {
 *     [](Move& mov) { ... }, // Reactions to each
 *     [](Attack& atk) { ... }
 * };
 * std::visit(r, a); // Calls `()` on the corresponding reaction
 * ```
 */
template<class... Ts>
struct Reactions : Ts... { using Ts::operator()...; };

}

#endif
