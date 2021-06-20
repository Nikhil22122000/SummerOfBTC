# SummerOfBTC
Coding Challenge:
The Problem is typically based on the Fractional Knapsack Problem.
Approach: Maintain a set of Transactions in the order of their fees to weight ratio and extract txn with maximum ratio if it is valid(Have no parents or all parents already included in the block),otherwise check for the next transaction,add it to the current block set and remove from the initial set.Do this until total weight is less than the given constraints or initial set is not empty.
