# Cube Maps and Skyboxes: Concepts, Rendering, and Resources

## **Overview**

This document covers the concepts of cube maps and skyboxes, their usage in rendering, and OpenGL cube map rendering order. Additionally, it includes layout details for horizontal and vertical cross cube maps and references useful resources.

---

## **What Are Cube Maps?**

A **cube map** is a texture consisting of six square images that represent the six faces of a cube. Cube maps are used to simulate a 360° environment around a point, enabling realistic reflections, refractions, and environment mapping.

### **Structure of a Cube Map**

- **Six Faces**:
  - **+X** (right)
  - **-X** (left)
  - **+Y** (top)
  - **-Y** (bottom)
  - **+Z** (front)
  - **-Z** (back)
- Each face corresponds to a specific 90° field of view (FOV).

### **Applications of Cube Maps**

1. **Environment Mapping**: Simulates reflections on shiny objects.
2. **Dynamic Reflections**: Updates cube maps in real time for moving objects.
3. **Image-Based Lighting (IBL)**: Provides ambient lighting in physically-based rendering (PBR).
4. **Shadow Mapping**: Used for omnidirectional shadow casting by point lights.

---

## **What Are Skyboxes?**

A **skybox** is a large cube or sphere surrounding the entire scene. It uses a cube map texture to create the illusion of a distant environment.

### **Applications of Skyboxes**

1. **Static Background Rendering**: Provides a seamless, static background (e.g., space or landscapes).
2. **Dynamic Effects**: Used for animated skyboxes, such as day-night cycles.
3. **Immersive Environments**: Enhances realism in VR and 3D applications.

### **Skybox Rendering Workflow**

1. Render the skybox before any scene objects.
2. Disable depth writing for the skybox to ensure it always appears in the background.
3. Use cube map sampling for seamless visuals.

---

## **OpenGL Cube Map Rendering Order**

In OpenGL, cube map faces are loaded in a specific order:

1. **+X** (Right): `GL_TEXTURE_CUBE_MAP_POSITIVE_X`
2. **-X** (Left): `GL_TEXTURE_CUBE_MAP_NEGATIVE_X`
3. **+Y** (Top): `GL_TEXTURE_CUBE_MAP_POSITIVE_Y`
4. **-Y** (Bottom): `GL_TEXTURE_CUBE_MAP_NEGATIVE_Y`
5. **+Z** (Front): `GL_TEXTURE_CUBE_MAP_POSITIVE_Z`
6. **-Z** (Back): `GL_TEXTURE_CUBE_MAP_NEGATIVE_Z`

To use cube maps in OpenGL, bind the six faces in the order above. For example:

```cpp
GLuint textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

std::vector<std::string> faces = {
    "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"
};

for (unsigned int i = 0; i < faces.size(); i++) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
}

// Set texture parameters
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
```

---

## **Cube Map Layouts**

### **Horizontal Cross Layout**

The horizontal cross layout arranges the cube map faces as follows:

```
+----+----+----+----+
|    | +Y |    |    |
+----+----+----+----+
| -X | +Z | +X | -Z |
+----+----+----+----+
|    | -Y |    |    |
+----+----+----+----+
```

### **Vertical Cross Layout**

The vertical cross layout arranges the cube map faces as follows:

```
+----+
| +Y |
+----+
| -Z |
+----+
| +X |
+----+
| +Z |
+----+
| -Y |
+----+
| -X |
+----+
```

---

## **Useful Resources**

1. **NASA Deep Star Maps**:
   Explore high-quality star maps for creating space backgrounds.

   - [NASA Deep Star Maps](https://svs.gsfc.nasa.gov/4851/)

2. **Convert .EXR to .HDR**:
   Tool to convert `.exr` files to `.hdr` format.

   - [FreeFileConvert](https://www.freefileconvert.com/exr-hdr)

3. **HDR to Cube Map**:
   Too to generate HDR image to cubemap.

   - [HDRI-to-CubeMap](https://matheowis.github.io/HDRI-to-CubeMap/)
