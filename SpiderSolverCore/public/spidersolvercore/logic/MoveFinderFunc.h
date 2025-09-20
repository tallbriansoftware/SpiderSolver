#pragma once

#include <functional>
#include <vector>

class MoveCombo;
class SpiderTableau;


using MoveFinderFunc = std::function<std::vector<MoveCombo>(const SpiderTableau&)>;
