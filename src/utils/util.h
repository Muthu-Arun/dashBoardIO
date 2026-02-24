#include <GL/gl.h>
#include <stb_image.h>

#include <cstddef>
#include <expected>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
namespace Utils {
namespace Log {
template <typename t>
void logVec(const std::vector<t>& vec) {
    std::cerr << std::format("Vector Size : {}", vec.size());
    std::cin.get();
    for (const t& elem : vec) {
        std::cerr << elem;
    }
    std::cerr << '\n';
}
}  // namespace Log
namespace Image {
inline std::expected<GLuint, std::string> genTextureFromImageBuffer(std::string& img_buf) {
    int w, h, channels;
    unsigned char* img = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(img_buf.data()),
                                               img_buf.size(), &w, &h, &channels, 4);

    if (!img) {
        return std::unexpected(std::string("error Loading image from memory"));
    }

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 4. Upload pixels to the GPU
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    stbi_image_free(img);
    return image_texture;
}

// Returns true on success, outputs the OpenGL texture ID and dimensions
inline bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width,
                                int* out_height) {
    // Load image data from file into CPU memory
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create an OpenGL texture
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // (Optional) Setup wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels to the GPU
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image_data);

    // Free the CPU memory now that the GPU has it
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
}  // namespace Image
}  // namespace Utils