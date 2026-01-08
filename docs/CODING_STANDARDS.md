# C++ Coding Standards - Poker Game Project

## Naming Conventions

### Classes
- **Format:** `PascalCase`
- **Example:** `PokerGame`, `PlayerManager`, `CardDeck`

### Methods
- **Format:** `snake_case`
- **Example:** `deal_cards()`, `calculate_hand_rank()`, `send_message()`

### Class Fields
- **Format:** `snake_case` with trailing underscore
- **Example:** `player_count_`, `current_bet_`, `card_deck_`

### Local Variables
- **Format:** `snake_case` with leading underscore
- **Example:** `_hand_value`, `_card_index`, `_is_valid`

### Function Parameters
- **Format:** 1-3 character abbreviations
- **Example:**
  ``` c++
  void deal_cards(int cnt, Player* plr, bool shf);
  int calculate_score(Hand* hnd, int mul);
  ```

## Example Implementation

``` c++
class PokerTable {
private:
    int player_count_;
    CardDeck* deck_;
    int current_bet_;

public:
    void add_player(Player* plr, int pos) {
        int _available_seats = get_available_seats();
        if (_available_seats > 0) {
            // Add player logic
        }
    }

    int calculate_pot(int bet, int cnt) {
        int _total = 0;
        int _multiplier = cnt;
        // Calculation logic
        return _total;
    }

    void deal_cards(CardDeck* dck, int num) {
        Card* _current_card = nullptr;
        // Deal logic
    }
};
```

## General Guidelines

### Code Organization
- One class per header file
- Separate interface (`.h`) from implementation (`.cpp`)
- Group related methods together

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Document all public methods with brief descriptions

### Formatting
- Indentation: 4 spaces (no tabs)
- Braces: Opening brace on same line for functions/classes
- Maximum line length: 100 characters

### Client/Server Specific
- Prefix client-side classes with `Client` (e.g., `ClientPokerGame`)
- Prefix server-side classes with `Server` (e.g., `ServerGameManager`)
- Use clear separation between networking and game logic

## Common Abbreviations

| Full Name | Abbreviation |
|-----------|--------------|
| player | plr |
| count | cnt |
| position | pos |
| hand | hnd |
| card | crd |
| deck | dck |
| message | msg |
| buffer | buf |
| connection | con |
| server | srv |
| client | cli |
| shuffle | shf |
| multiplier | mul |
| number | num |