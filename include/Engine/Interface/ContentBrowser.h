#ifndef SKYNET_INTERFACE_CONTENTBROWSER_H
#define SKYNET_INTERFACE_CONTENTBROWSER_H

#include "System.h"
#include "Texture.h"

namespace Skynet
{
    class ContentBrowser
    {
    public:
        ContentBrowser();
        ~ContentBrowser();

        void OnRender();

    private:
        std::filesystem::path curdir;
        
        ref<Texture2D> folderIcon;
        ref<Texture2D> fileIcon;
    };

} // namespace Skynet


#endif // SKYNET_INTERFACE_CONTENTBROWSER_Ha