#pragma once

class MoveCombo;
class MoveSingle;
class SpiderTableau;

#ifdef _DEBUG
bool IsMoveLegal(const SpiderTableau& tableau, int fromStack, int fromIndex, int destStack);
bool IsMoveLegal(const SpiderTableau& tableau, int fromStack, int fromIndex, int destStack, int destIndex);
bool IsMoveLegal(const SpiderTableau& tableau, const MoveSingle& move);
bool IsMoveLegal(const SpiderTableau& tableau, const MoveCombo& move);
#endif
