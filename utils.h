#pragma once

#include <string>
#include <vector>

namespace utils
{
    /** Split a string along a given character. The character to split on is not included in the output.
     * @param toSplit The string to split
     * @param splitOn The character to split on
     * @return the line, split along the characters
     */
    std::vector<std::string> split(const std::string &toSplit, char splitOn);

    /** Calculate the time in minutes from midnight given a string of the form 0015 (0 hours, 15 mins)
     */
    size_t timeFromMidnight(std::string &time);

    /** Return the time as an hh:mm string 
     * @param minsFromMidnight time in minutes from midnight
     */
    std::string printTime(size_t minsFromMidnight);

    /** Multiply a matrix and a vector. The length of the matrix must be valid.
     * @param matrix an nxm vector.
     * @param vector a vector of length m.
     * @throws -1 if the matrix and vector cannot be multiplied.
     * @return a vector that is equal to Ax (matrix-vector multiplication).
     */
    std::vector<double> matrixVectorProduct(std::vector<std::vector<double>> &matrix, std::vector<double> &vector);

    /** Normalize a vector with the 1-norm (make its components sum to 1).
     * @param vec The vector to normalize
     * @return The same vector, but normalized
     */
    std::vector<double> normalize(std::vector<double> &vec);

    /** Perform power iteration on a matrix.
     * @param matrix an nxn vector.
     * @param vector a vector of length n.
     * @param iter the number of iterations.
     * @throws -1 if the matrix and vector cannot be multiplied.
     * @return a vector that is equal to (A^k)x (matrix-vector multiplication). 
     * This vector approximates an eigenvector of the matrix.
     */
    std::vector<double> powerIteration(std::vector<std::vector<double>> &matrix, std::vector<double> &vector, size_t iter);

} // namespace utils
