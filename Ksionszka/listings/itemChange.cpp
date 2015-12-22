QVariant VertexImage::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// ...
	else if (change == ItemSelectedChange)
	{
		if (value.toBool() == true)
		{
			_context = getParent()->getConfig()->SelectedVertexContext();
		}
		else
		{
			_context = getParent()->getConfig()->NormalVertexContext();
		}
	}
}