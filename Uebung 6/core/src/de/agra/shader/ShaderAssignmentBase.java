package de.agra.shader;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Mesh;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.VertexAttribute;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Vector3;

import java.io.IOException;

public class ShaderAssignmentBase extends ApplicationAdapter {
	SpriteBatch batch;
	Texture img;
	public PerspectiveCamera cam;
    public ShaderProgram shader;

	/**
	 * Wir nutzen statt einzelnen Variablen für jedes Mesh ein Mesh-Array, das bequem durchiteriert werden kann.
	 */
	public Mesh [] meshList = new Mesh[3];

	/**
	 * X-Offset aller Meshes.
	 * Wird genutzt, um die Meshes zu zentrieren und auf der X-Achse beliebig verschiebbar zu machen.
	 */
	private final float meshStartOffX = -1.2f;

	/**
	 * X-Offset zwischen einzelnen Meshes.
	 * Wird mit der Nummer der aktuellen Meshes multipliziert, um Formen nebeneinander darzustellen.
	 */
	private final float meshOffX = java.lang.Math.abs (meshStartOffX);

	@Override
	public void create () {
		batch = new SpriteBatch();
		img = new Texture("agra.png");
        FileHandle vertexShaderH = new FileHandle("vertexShader.prg");
        FileHandle fragmentShaderH = new FileHandle("fragmentShader.prg");

        shader = new ShaderProgram(vertexShaderH, fragmentShaderH);


		//Fülle die Mesh-Liste mit den gewünschten Formen.
		for (int i = 0; i<meshList.length; i++){
				meshList[i] = getMesh(i, i);
		}

		
        cam = new PerspectiveCamera(20, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        cam.position.set(0f, 0f, 10f);
        cam.lookAt(0,0,0);
        cam.near = 1f;
        cam.far = 300f;
        cam.update();
	}

	/**
	 * Gibt ein Mesh zurück, dessen Vertices und Indices bereits definiert wurden.
	 * @param typeNumber - Pyramide (0), Zylinder (1) oder Kugel (2)?
	 *                   Bei allen anderen Zahlen wird stattdessen das vorgegebene Logo gezeigt.
	 * @param meshNumber - Der wievielte gezeigte Mesh ist dies?
	 *                   Wird zum Offset der Mesh-Koordinaten genutzt.
     */
	private Mesh getMesh (int typeNumber, int meshNumber){
		switch(typeNumber){
			default:
				return getMeshLogo (meshNumber);
		}
	}

	private Mesh getMeshLogo (int meshNumber){
		/**
		 * Modifier aller x-Koordinaten.
		 */
		float xm = meshStartOffX + meshNumber * meshOffX;

		Mesh mesh;

		mesh = new Mesh(true, 4, 6, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(), VertexAttribute.TexCoords(0));
		mesh.setVertices(new float[]
				// Position XYZ   Color RGBA   Texture Coordinates UV
				//|------------|  |--------|  |--|
				{-0.5f+xm, -0.5f, 0, 1, 1, 1, 1, 0, 1,
						0.5f+xm, -0.5f, 0, 1, 1, 1, 1, 1, 1,
						0.5f+xm,  0.5f, 0, 1, 1, 1, 1, 1, 0,
						-0.5f+xm,  0.5f, 0, 1, 1, 1, 1, 0, 0});
		mesh.setIndices(new short[] {0, 1, 2, 2, 3, 0});

		return mesh;
	}

	@Override
	public void render () {
		Gdx.gl.glClearColor(0.25f, 0.25f, 0.5f, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		
		cam.position.set(
				(Gdx.input.getX() - Gdx.graphics.getWidth() / 2f) / 100f,
				(Gdx.input.getY() - Gdx.graphics.getHeight() / 2f) / 100f,
				10f);
		cam.up.set(0,1,0);
		cam.lookAt(0,0,0);
		cam.update();
		
		img.bind();
		shader.begin();
		shader.setUniformMatrix("u_worldView", cam.combined);
		shader.setUniformi("u_texture", 0);
		for (int i = 0; i<meshList.length; i++){
				meshList[i].render(shader, GL20.GL_TRIANGLES);
		}
		//mesh.
		shader.end();
	}
	
	@Override
	public void resize(int width, int height) {
        cam.viewportHeight = Gdx.graphics.getHeight();
        cam.viewportWidth = Gdx.graphics.getWidth();
    }
}
