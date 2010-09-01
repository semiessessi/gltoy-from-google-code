// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/JD1.h>

// This file's header
#include <Demo/JD1_DemoScene_TexerFlow.h>

// GLToy
#include <Render/GLToy_Render.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

JD1_DemoScene_TexerFlow::JD1_DemoScene_TexerFlow()
: GLToy_Parent()
, m_xTexer()
, m_xBlur()
, m_xFlow()
{
}

void JD1_DemoScene_TexerFlow::Initialise()
{
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xFlow ), static_cast< GLToy_Updateable* >( &m_xFlow ) );
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xBlur ), static_cast< GLToy_Updateable* >( &m_xBlur ) );
    AppendComponent( static_cast< GLToy_Renderable* >( &m_xTexer ), static_cast< GLToy_Updateable* >( &m_xTexer ) );

    GLToy_Parent::Initialise();
}

void JD1_DemoScene_TexerFlow::Start()
{
    GLToy_Parent::Start();

    GLToy_Render::SetClearFrame( true );
    GLToy_Render::Clear();
}

void JD1_DemoScene_TexerFlow::Update()
{
    GLToy_Parent::Update();

    if( m_fTimer > 7.0f )
    {
        GLToy_Render::SetClearFrame( false );
    }
}
