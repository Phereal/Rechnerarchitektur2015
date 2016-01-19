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

public class ShaderAssignmentBase extends ApplicationAdapter {
	SpriteBatch batch;
	Texture img;
	public PerspectiveCamera cam;
    public Mesh mesh;
    public ShaderProgram shader;


	@Override
	public void create () {
		batch = new SpriteBatch();
		img = new Texture("agra.png");
        FileHandle vertexShaderH = new FileHandle("vertexShader.prg");
        FileHandle fragmentShaderH = new FileHandle("fragmentShader.prg");

        shader = new ShaderProgram(vertexShaderH, fragmentShaderH);
		
		mesh = new Mesh(true, 4, 6, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(), VertexAttribute.TexCoords(0));
		mesh.setVertices(new float[] 
		// Position XYZ   Color RGBA   Texture Coordinates UV   
		//|------------|  |--------|  |--|
		{-0.5f, -0.5f, 0, 1, 1, 1, 1, 0, 1,
		  0.5f, -0.5f, 0, 1, 1, 1, 1, 1, 1,
		  0.5f,  0.5f, 0, 1, 1, 1, 1, 1, 0,
		 -0.5f,  0.5f, 0, 1, 1, 1, 1, 0, 0});
		mesh.setIndices(new short[] {0, 1, 2, 2, 3, 0});
		
        cam = new PerspectiveCamera(20, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        cam.position.set(0f, 0f, 10f);
        cam.lookAt(0,0,0);
        cam.near = 1f;
        cam.far = 300f;
        cam.update();
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
		mesh.render(shader, GL20.GL_TRIANGLES);
		shader.end();
	}
	
	@Override
	public void resize(int width, int height) {
        cam.viewportHeight = Gdx.graphics.getHeight();
        cam.viewportWidth = Gdx.graphics.getWidth();
    }
}
