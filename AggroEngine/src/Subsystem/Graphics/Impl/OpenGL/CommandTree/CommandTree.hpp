#pragma once

#include <memory>
#include <vector>
#include "Layer/Layer.hpp"
#include "RenderOptions.hpp"
#include "RenderChain.hpp"
using namespace std;

/* This object aims to minimize the amount of duplicate work required for rendering.
 * In particular, it trys to reduce the number of OpenGL state changes, but can also 
 * be used to reduce the number of expensive CPU operations such as matrix multiplication.
 *
 * An example tree as follows
 *                                  
 *  Disable Depth Test     |                   false                    |                  true              |
 *  Shader Binding         |     shdr_a      |   shdr_b   |   shdr_c    |       shdr_b       |      shdr_d   |
 *  Texture Unit 0 Binding | tex1 tex4 tex23 |  tex4 tex7 |     ...     |         ...        |      ...      |
 *  Texture Unit 1 Binding |                     ...                    |                   ...              |
 *  Texture Unit 2 Binding |                                            |                                    |
 *  Matrix Uniforms        |                                            |                                    |
 *  Matrial Uniforms       |                                            |                                    |
 *  Draw Elements          |                                            |                                    |
 *
 *
 * A cost guide from NVIDIA here: https://i.stack.imgur.com/JgrSc.jpg

 * Basically: Render Target > Program > ROP > Texture Bindings > 
 *            Vertex Format > UBO Bindings > Vertex Bindings > Uniform Updates
 */
class CommandTree
{
private:
	vector<shared_ptr<Layer>> m_layers;

	void _executeLayer(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes, int layerIdx);

public:
	CommandTree();

	void addLayer(shared_ptr<Layer> layer);
	void execute(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain);
};