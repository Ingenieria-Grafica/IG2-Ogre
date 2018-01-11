vertex_program vertexShaderE2 {

	source fragmentShaderE2glsl.txt; // archivo

	default_params //valores de las variables uniform
	{
		param_named textura1 int 0;//unidad de textura
		param_named textura2 int 1;
		param_named FB float 0.5;
		param_named intLuz float 1.0;
	}
}

