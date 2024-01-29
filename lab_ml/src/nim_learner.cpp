/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    unsigned ctoken = startingTokens;
    for (unsigned i = 0; i <= ctoken; i++) {
        std::string player1 = "p1-" + std::to_string(i);
        std::string player2 = "p2-" + std::to_string(i);
        g_.insertVertex(player1);
        g_.insertVertex(player2);

        if (i > 0) {
            g_.insertEdge(player1, "p2-" + std::to_string(i - 1));
            g_.insertEdge(player2, "p1-" + std::to_string(i - 1));
            g_.setEdgeWeight(player1, "p2-" + std::to_string(i - 1), 0);
            g_.setEdgeWeight(player2, "p1-" + std::to_string(i - 1), 0);
        }
        if (i > 1) {
            g_.insertEdge(player1, "p2-" + std::to_string(i - 2));
            g_.insertEdge(player2, "p1-" + std::to_string(i - 2));
            g_.setEdgeWeight(player1, "p2-" + std::to_string(i - 2), 0);
            g_.setEdgeWeight(player2, "p1-" + std::to_string(i - 2), 0);
        }
    }
}


/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */

std::vector<Edge> NimLearner::playRandomGame() const {
  std::vector<Edge> path;
  unsigned tokens = 0;

  for (const Vertex &v : g_.getVertices()) {
    size_t temp1 = v.find('-');
    if (temp1 < v.length()) {
      unsigned vertex = 0;
      for (size_t i = temp1 + 1; i < v.length(); ++i) {
        vertex = vertex * 10 + (v[i] - '0');
      }
      if (vertex > tokens) {
        tokens = vertex;
      }
    }
  }

  std::string cplayer = "p1";

  while (tokens > 0) {
    unsigned taketokens = (rand() % 2) + 1;
    if (taketokens > tokens) {
      taketokens = tokens;
    }

    std::stringstream stringg;
    stringg << (cplayer == "p1" ? "p2" : "p1") << "-" << (tokens - taketokens);
    std::string nextState = stringg.str();

    std::stringstream stream;
    stream << cplayer << "-" << tokens;
    std::string currentState = stream.str();

    path.push_back(g_.getEdge(currentState, nextState));

    tokens -= taketokens;
    cplayer = (cplayer == "p1" ? "p2" : "p1");
  }

  return path;
}


  

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
    if (path.empty()) return;

    std::string temp1 = path.back().dest;
    bool win = temp1 == "p2-0";

    for (const Edge &edge : path) {
        bool edgee = edge.source.substr(0, 2) == "p1";
        int weight = g_.getEdgeWeight(edge.source, edge.dest);

        if ((win && edgee) || (!win && !edgee)) {
            g_.setEdgeWeight(edge.source, edge.dest, weight + 1);
        } else {
            g_.setEdgeWeight(edge.source, edge.dest, weight - 1);
        }
    }
}



/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
