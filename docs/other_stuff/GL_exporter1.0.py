import math
import Blender210
import GUI

# GL Export v1.0 by
# Rogerio Mazakina
# dway@designer.mailbr.com.br
# http://dway.cjb.net
# Please do not edit this header

# press Alt+P to exec
# then choose the name of the file
# that will be exported
# See readme.txt for more information

def getNormal(A,B,C):
    M_ax = M_ay = M_az = 0
    M_bx = M_by = M_bz = 0
    M_nx = M_ny = M_nz = 0
    M_Len = 0

    M_ax = B[0] - A[0]
    M_ay = B[1] - A[1]
    M_az = B[2] - A[2]

    M_bx = C[0] - A[0]
    M_by = C[1] - A[1]
    M_bz = C[2] - A[2]

    M_nx = (M_ay * M_bz) - (M_by * M_az)
    M_ny = (M_az * M_bx) - (M_bz * M_ax)
    M_nz = (M_ax * M_by) - (M_bx * M_ay)
# get length of normal
    M_Len = (M_nx * M_nx) + (M_ny * M_ny) + (M_nz * M_nz)

    if M_Len < 0:
        M_Len = math.sqrt(-M_Len)
        M_Len = -M_len
    elif M_Len > 0: M_Len = math.sqrt(M_Len)
# normalize the normal
    if M_Len != 0:
        M_nx = M_nx/M_Len
        M_ny = M_ny/M_Len
        M_nz = M_nz/M_Len

    M_normal=[]
    M_normal.append(M_nx)
    M_normal.append(M_ny)
    M_normal.append(M_nz)
    return M_normal
# END OF FUNCTION getNormal(A,B,C)

print "####################################"
print "        GL file Export Script"

def GLexport(filename):
  model = ""
  filename = filename+".c"
  file = open(filename,"w")
  scene = Blender210.getCurrentScene()

  for names in scene.objects:
      obj = Blender210.getObject(names)
      if obj.type == "Mesh":
          mesh = Blender210.getMesh(obj.data)
          model = obj.data
          break

  print " Exporting %s now." %model

  file.write("void model()\n{\n")
  file.write("// Object name: %s\n" %model)
  file.write("// File Exported by GLexport Script.\n")
  FaceIndex = 0
  for face in mesh.faces:
      if face[3] == 0: file.write("  glBegin(GL_TRIANGLES);\n")
      else:            file.write("  glBegin(GL_QUADS);\n")

      if face[4] == 0:
          nx,ny,nz = getNormal(mesh.vertices[face[0]],
                               mesh.vertices[face[1]],
                               mesh.vertices[face[2]])
          file.write("    glNormal3f(%3.6f, %3.6f, %3.6f);\n" % (nx,ny,nz))

      vCount = 0
      for vCount in range(4):
          if vCount == 3 and face[vCount] == 0:   break

          if face[4] == 1: # if the face is Smooth
              file.write("    glNormal3f(%3.6f, %3.6f, %3.6f);\n" %
                        (mesh.normals[face[vCount]][0],
                         mesh.normals[face[vCount]][1],
                         mesh.normals[face[vCount]][2] ));
          if mesh.texture: # if the mesh have texture (only UV coords)
              file.write("    glTexCoord2f(%3.6f,%3.6f);" %
                        (mesh.texcoords[FaceIndex][vCount][0],
                         mesh.texcoords[FaceIndex][vCount][1] ))
          file.write("    glVertex3f(%3.6f, %3.6f, %3.6f);\n" %
                    (mesh.vertices[face[vCount]][0],
                     mesh.vertices[face[vCount]][1],
                     mesh.vertices[face[vCount]][2] ))
      #} END FOR
      file.write("  glEnd();\n\n")
      FaceIndex = FaceIndex+1
  #} END FOR FACES
  file.write("}\n")
  file.close()
  print " ..."
  print " Export done!"
print "-------  END GL EXPORT.PY  ----------"

def callback():
  GLexport(fs.filename)

fs = GUI.FileSelector()
fs.activate(callback)